//
// Created by chao on 2018/3/26.
//
#define LOG_TAG "CameraMediaSourceOnAndroid"
#include <BaseType.h>
#include <gl/RenderEngine.h>
#include <DebugHelper.h>
#include <gl/Texture.h>
#include "CameraMediaSourceOnAndroid.h"
#include "j4a/camera_wrapper.h"

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

    jobject camInst = J4AC_com_media_gankers_medianative_CameraWrapper__CameraWrapper__catchAll(helper.env(), (int64_t) this);
    JNIHelper::LocalRefObject localRef(camInst);

        if (camInst) {
            int isFront = -1;
            int idx = 0;
            mMetaData->findInt32(kKeyCamSourceFront, &isFront);

            if (isFront) {
                idx = J4AC_com_media_gankers_medianative_CameraWrapper__getFrontCameraIdx__catchAll(helper.env());
            } else {
                idx = J4AC_com_media_gankers_medianative_CameraWrapper__getBackCameraIdx__catchAll(helper.env());
            }

            bool opened = J4AC_com_media_gankers_medianative_CameraWrapper__open__catchAll(helper.env(), camInst, idx);
            if (opened) {
                int oesTex = getOESTexture();
                bool startPreviewed = J4AC_com_media_gankers_medianative_CameraWrapper__startPreview__catchAll(helper.env(), camInst, oesTex);
                ret = startPreviewed ? 0 : ERRNUM();
            }
        }

    if (!ret) {
        mCamJava.setObj(camInst);
        setStatus(kPreared);
    } else {
        setStatus(kError);
        char buffer[1024] = {0};
        const char *errStr = J4AC_com_media_gankers_medianative_CameraWrapper__getExceptionMessage__asCBuffer__catchAll(helper.env(), camInst, buffer, 1024);

        if (errStr) {
            ALOGE("Preparing camera failed [%s]", errStr);
            mMetaData->dumpToLog();
        }
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
        MAKE_MSG_SYNC(msg, kMsgReleaseSource, nullptr);
        POSTANDWAIT(msg.msg);

        mRenderQueue.requestExit(true);

        if (mRenderEngine) {
            mRenderEngine->release();
            mRenderEngine = nullptr;
        }

        if (mMesh) {
            delete mMesh;
            mMesh = nullptr;
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

    mMetaData->dumpToLog();
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
                                                           mRenderQueue(10, true) {
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

    SCOPEDDEBUG();
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

            delete mTexPool;
            mTexPool = nullptr;
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
    SCOPEDDEBUG();
    ALOGI("STONE onFrameAvailable %p", this);
    mCameraTex.setMatrix(matrix);
    MAKE_MSG(msg, kMsgOnFrameAvailable);
    POST(msg.msg);
    return 0;
}

void CameraMediaSourceOnAndroid::createRenderEngine() {

    mRenderEngine = openGl::RenderEngine::createWithContext();

    mEglSurface = mRenderEngine->createSurface(1, 1);

    mRenderEngine->makeCurrent(mEglSurface);

    //mRenderEngine->setupLayerBlending(false, false, 1.0f);

    mMesh = new openGl::Mesh(openGl::Mesh::TRIANGLE_FAN, 4, 2, 2);

    openGl::Mesh::VertexArray<openGl::vec2> position = mMesh->getPositionArray<openGl::vec2>();
    openGl::Transform tr;
    position[1] = tr.transform(1, -1);
    position[2] = tr.transform(-1, -1);
    position[3] = tr.transform(-1, 1);
    position[0] = tr.transform(1, 1);

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
    mTexPool = new TexBufferPool(1, w, h, "CameraTexPool");

    GlUtils::createFrameBuffer(1, &mFbo);
}

void CameraMediaSourceOnAndroid::draw() {
    SCOPEDDEBUG();
    JNIHelper helper;

    {
        std::lock_guard<std::mutex> lock_guard(mSyncLock);

        if (mCamJava.mObj) {
            SCOPEDDEBUG();
            J4AC_com_media_gankers_medianative_CameraWrapper__updateImage(helper.env(),
                                                                          mCamJava.mObj);
        }
    }


}

int CameraMediaSourceOnAndroid::stopInternal() {
    SCOPEDDEBUG();
    JNIHelper helper;

    if (mCamJava.mObj) {
        J4AC_com_media_gankers_medianative_CameraWrapper__release(helper.env(), mCamJava.mObj);
    }

    return 0;
}

CameraMediaSourceOnAndroid::~CameraMediaSourceOnAndroid() {
    SCOPEDDEBUG();
    release();
}

void CameraMediaSourceOnAndroid::drawWithMatrix(jlong ts, jfloat *matrix) {

    // already locked

    SCOPEDDEBUG();
    if (status() != kStarted) {
        SCOPEDDEBUG();
        return;
    }

    if (!ts || mTexPool == nullptr || matrix == nullptr) {
        SCOPEDDEBUG();
        return;
    }

    mCameraTex.setMatrix(matrix);
    TexBuffer *texBuffer = mTexPool->pollAsTexBuffer(200);

    if (!texBuffer) {
        ALOGE("No enough texture buffer, skip frame");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texBuffer->texId(),
                           0);
    mRenderEngine->setupLayerTexturing(mCameraTex);
    glViewport(0, 0, 1280, 720);
    mRenderEngine->drawMesh(*mMesh);
    //mRenderEngine->clearWithColor(rand() / (double)RAND_MAX, rand() / (double)RAND_MAX, rand() / (double)RAND_MAX, 1.f);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ALOGI("setupLayerTexture on camera tex %d w %d h %d ", texBuffer->texId(), texBuffer->width(), texBuffer->height());

    if (!deliver(texBuffer, StreamType::kStreamVideo)) {
        ALOGW("Not found deliver !");
    }
    texBuffer->decRef();
}

