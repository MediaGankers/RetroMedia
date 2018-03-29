//
// Created by chao on 2018/3/26.
//

#include <BaseType.h>
#include <gl/RenderEngine.h>
#include <DebugHelper.h>
#include <gl/Texture.h>
#include "CameraMediaSourceOnAndroid.h"


struct MessagePrivate {
    MessageQueue::Msg msg;
    volatile bool alreadCall;
    bool sync;
    void *data;
};

#define MAKE_MSG(name, w)     \
   MessagePrivate name; \
   name.msg.hwd = this;        \
   name.msg.what = w; \
   name.msg.data = nullptr; \
   name.alreadCall = false; \
   name.sync = false; \
   name.data = nullptr

#define MAKE_MSG_SYNC(name, w, d) \
  MAKE_MSG(name, w); \
  name.msg.data = &name; \
  name.sync = true; \
  name.data = d


#define POST(msg) \
 mRenderQueue.post(msg)

#define POSTANDWAIT(msg)   \
    std::unique_lock<std::mutex> lock_guard(mSyncLock); \
    mRenderQueue.post(msg); \
    MessagePrivate *p = (MessagePrivate *)msg.data; \
    while(p && !p->alreadCall) { \
       mSyncCond.wait(lock_guard); \
    }

int CameraMediaSourceOnAndroid::start() {
    std::lock_guard<std::mutex> lockGuard(mStatLock);

    switch (status()) {
        case kStarted:
        case kPause:
        case kPreared:
            break;
        default:
            return ERRNUM();
    }

    return MediaSource::start();
}

int CameraMediaSourceOnAndroid::stop() {
    SCOPEDDEBUG();
    JNIHelper helper;
    std::lock_guard<std::mutex> lockGuard(mStatLock);

    int ret = ERRNUM();
    switch (status()) {
        case kStarted:
        case kPause:
        case kPreared:
        case kInit:
        case kPreparing:
            if (!stopInternal()) {
                MediaSource::stop();
                ret = 0;
            } else {
                MediaSource::setStatus(kError);
                ret = ERRNUM();
            }

            break;
        default:
            ret = 0;
            break;

    }
    return ret;
}

int CameraMediaSourceOnAndroid::pause() {
    std::lock_guard<std::mutex> lockGuard(mStatLock);

    if (status() == kStarted || status() == kPause) {
        return MediaSource::pause();
    }

    return ERRNUM();
}

int CameraMediaSourceOnAndroid::prepare() {
    JNIHelper helper;
    std::lock_guard<std::mutex> lockGuard(mStatLock);
    Status s = status();

    int ret = ERRNUM();

    switch (s) {
        case kStoped:
        case kInit:
            break;
        default:
            return ERRNUM();
    }

    setStatus(kPreparing);

    static jclass jcls = helper.env()->FindClass("com/media/gankers/medianative/CameraWrapper");
    static jmethodID jStartPeview = jcls ? helper.env()->GetMethodID(jcls, "startPreview", "(I)Z")
                                         : nullptr;
    static jmethodID jopen = jcls ? helper.env()->GetMethodID(jcls, "open", "(I)Z") : nullptr;
    static jmethodID jrelease = jcls ? helper.env()->GetMethodID(jcls, "release", "()V") : nullptr;
    static jmethodID jconstruct = jcls ? helper.env()->GetMethodID(jcls, "<init>", "(J)V") : nullptr;
    static jmethodID jgetFrontCamIdx = jcls ? helper.env()->GetStaticMethodID(jcls,
                                                                              "getFrontCameraIdx",
                                                                              "()I") : nullptr;
    static jmethodID jgetBackCamIdx = jcls ? helper.env()->GetStaticMethodID(jcls,
                                                                             "getBackCameraIdx",
                                                                             "()I") : nullptr;

    JNIHelper::LocalRefObject localRef(nullptr);
    if (jconstruct) {
        localRef.setObj(helper.env()->NewObject(jcls, jconstruct, (int64_t) this));

        if (localRef.mObj) {
            int isFront = -1;
            int idx = 0;
            mMetaData->findInt32(kKeyCamSourceFront, &isFront);

            if (isFront) {
                idx = helper.env()->CallStaticIntMethod(jcls, jgetFrontCamIdx);
            } else {
                idx = helper.env()->CallStaticIntMethod(jcls, jgetBackCamIdx);
            }

            if (helper.env()->CallBooleanMethod(localRef.mObj, jopen, idx)) {
                int oesTex = getOESTexture();
                ret = helper.env()->CallBooleanMethod(localRef.mObj, jStartPeview, oesTex) ? 0
                                                                                           : ERRNUM();
            }
        }
    }

    if (!ret) {
        mCamJava.setObj(localRef.mObj);
        setStatus(kPreared);
    } else {
        setStatus(kError);
    }

    return ret;
}

int CameraMediaSourceOnAndroid::release() {
    SCOPEDDEBUG();

    {
        std::lock_guard<std::mutex> lockGuard(mStatLock);
        if (status() == kEnd) {
            return 0;
        }
    }

    reset();

    {
        std::lock_guard<std::mutex> lockGuard(mStatLock);
        MAKE_MSG(msg, kMsgReleaseSource);
        POST(msg.msg);

        mRenderQueue.requestExit(true);

        if (mRenderEngine) {
            mRenderEngine->release();
            mRenderEngine = nullptr;
        }

    }
    return MediaSource::release();
}

int CameraMediaSourceOnAndroid::setParameter(int key, void *obj) {
    int ret = ERRNUM();

    if (!mMetaData) {
        return ret;
    }

    switch (key) {
        case kKeyWidth:
        case kKeyHeight:
        case kKeyFrameRate:
            ret = MediaSource::setParameter(key, obj);
            break;
        case kKeyCamSourceFront:
            setStatus(kInit);
        case kKeyCamSourceMirror:
            mMetaData->setInt32(key, *((int *) obj));
            ret = 0;
            break;
    }
    return ret;
}

int CameraMediaSourceOnAndroid::reset() {
    SCOPEDDEBUG();
    stop();
    std::lock_guard<std::mutex> lockGuard(mStatLock);
    MediaSource::reset();
    return 0;
}

CameraMediaSourceOnAndroid::CameraMediaSourceOnAndroid() : CameraMediaSource(),
                                                           mRenderQueue(100, true) {
    SCOPEDDEBUG();
    mRenderEngine = nullptr;
    mTexPool = nullptr;

    MAKE_MSG(msg, kMsgCreateRenderEngine);
    POST(msg.msg);
}

int CameraMediaSourceOnAndroid::getOESTexture() {
    SCOPEDDEBUG();

    int tex = 0;
    MAKE_MSG_SYNC(msg, kMsgCreateOESTExture, &tex);
    POSTANDWAIT(msg.msg);
    return tex;
}

void CameraMediaSourceOnAndroid::process(MessageQueue::Msg *msg) {

    MessagePrivate *box = (MessagePrivate *) msg->data;
    switch (msg->what) {
        case kMsgCreateRenderEngine: {
            createRenderEngine();
            break;
        }
        case kMsgCreateOESTExture: {
            if (mRenderEngine) {

                if (mCameraTex.getTextureName() <= 0) {
                    uint32_t oesTex = 0;
                    mRenderEngine->genTextures(1, &oesTex);
                    mCameraTex.init(openGl::Texture::TEXTURE_EXTERNAL, oesTex);
                }

                mRenderEngine->setupLayerTexturing(mCameraTex);
                if (box && box->data) {
                    *((uint32_t *) box->data) = mCameraTex.getTextureName();
                }
                break;
            }
        }

        case kMsgOnFrameAvailable: {
            draw();
            break;
        }

        case kMsgReleaseSource: {
            GLuint tex = mCameraTex.getTextureName();
            if (tex > 0) {
                GlUtils::deleteTexture(1, &tex);
                tex = 0;
            }

            if (mFbo > 0) {
                GlUtils::deleteFrameBuffer(1, &mFbo);
                mFbo = 0;
            }

            mRenderEngine->destroySurface(mEglSurface);

            mRenderEngine->release();
            mRenderEngine = nullptr;
            break;
        }

    }

    if (box && box->sync) {
        std::lock_guard<std::mutex> lock_guard(mSyncLock);
        box->alreadCall = true;
        mSyncCond.notify_all();
    }

}

int CameraMediaSourceOnAndroid::onFrameAvailable(int64_t timestamp, float *matrix) {
    mCameraTex.setMatrix(matrix);
    MAKE_MSG(msg, kMsgOnFrameAvailable);
    POST(msg.msg);
    return 0;
}

void CameraMediaSourceOnAndroid::createRenderEngine() {

    mRenderEngine = openGl::RenderEngine::create(EGL_NO_CONTEXT);

    mEglSurface = mRenderEngine->createSurface(1, 1);

    mRenderEngine->makeCurrent(mEglSurface);

    mRenderEngine->setupLayerBlending(false, false, 1.0f);

    mMesh = new openGl::Mesh(openGl::Mesh::TRIANGLE_FAN, 4, 2, 2);

    openGl::Mesh::VertexArray<openGl::vec2> position = mMesh->getPositionArray<openGl::vec2>();
    openGl::Transform tr;
    position[0] = tr.transform(-1, 1);
    position[1] = tr.transform(-1, -1);
    position[2] = tr.transform(1, 1);
    position[3] = tr.transform(1, -1);

    tr.reset();
    openGl::Mesh::VertexArray<openGl::vec2> coord = mMesh->getTexCoordArray<openGl::vec2>();

    coord[0] = tr.transform(0, 1);
    coord[1] = tr.transform(0, 0);
    coord[2] = tr.transform(1, 0);
    coord[3] = tr.transform(1, 1);

    int w = 0;
    int h = 0;
    mMetaData->findInt32(kKeyWidth, &w);
    mMetaData->findInt32(kKeyHeight, &h);

    ALOGI("Find Camera w %d h %d", w, h);

    w = w > 0 ? w : 1280;
    h = h > 0 ? h : 720;
    mTexPool = new TexBufferPool(10, w, h, "CameraTexPool");

    GlUtils::createFrameBuffer(1, &mFbo);
}

void CameraMediaSourceOnAndroid::draw() {
    SCOPEDDEBUG();
    JNIHelper helper;
    static jclass jcls = helper.env()->GetObjectClass(mCamJava.mObj);
    static jmethodID jupdateImage = jcls ? helper.env()->GetMethodID(jcls, "updateImage", "()J")
                                         : nullptr;

    long ts = 0;

    if (jupdateImage) {
        ts = helper.env()->CallLongMethod(mCamJava.mObj, jupdateImage);
    }

    if (!ts) {
        SCOPEDDEBUG();
        return;
    }

    if (status() != kStarted) {
        SCOPEDDEBUG();
        return;
    }

    TexBuffer *texBuffer = mTexPool->pollAsTexBuffer();

    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBuffer->texId(),
                           0);
    mRenderEngine->drawMesh(*mMesh);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    deliver(texBuffer, StreamType::kStreamVideo);
    texBuffer->decRef();
}

int CameraMediaSourceOnAndroid::stopInternal() {
    SCOPEDDEBUG();
    JNIHelper helper;
    static jclass jcls = helper.env()->FindClass("com/media/gankers/medianative/CameraWrapper");
    static jmethodID jrelease = jcls ? helper.env()->GetMethodID(jcls, "release", "()V") : nullptr;

    if (mCamJava.mObj) {
        if (jrelease) {
            helper.env()->CallVoidMethod(mCamJava.mObj, jrelease);
            mCamJava.setObj(nullptr);
        }
    }

    return 0;
}

CameraMediaSourceOnAndroid::~CameraMediaSourceOnAndroid() {
    SCOPEDDEBUG();
    release();
}

