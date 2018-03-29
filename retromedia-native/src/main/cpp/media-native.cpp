#include <jni.h>
#include <string>
#include <log_defs.h>
#include <JNIHelper.h>
#include <DebugHelper.h>
#include "VideoSource.h"
#include "Decoder.h"
#include "common/TexBufferPool.h"
#include "source/MediaSource.h"
#include "source/CameraMediaSource.h"
#include "source/android/CameraMediaSourceOnAndroid.h"
#include "j4a/camera_wrapper.h"
extern "C"

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIHelper::setupJVM(vm);

    JNIHelper helper;
    J4A_loadClass__J4AC_com_media_gankers_medianative_CameraWrapper(helper.env());
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {

}

extern "C"
JNIEXPORT jstring
        JNICALL
Java_com_media_gankers_medianative_MediaNative_helloFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from JNI, let's make it awesome";
//    IVideoSource *source = VideoSource::create(kVideoSourceFile);
//    source->setSource("/sdcard/BasketballDrill_832x480_50.yuv");
//    source->setSourceInfo(832, 480, kVideoPictureFormatI420);
//    source->open();
//    source->start();
//
//    ///////////
//    Decoder *decoder = new Decoder;
//    decoder->start("/sdcard/test.mp4");
//    decoder->resume();
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_VideoBuffer_size(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->size();
    }
    return 0;

}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_VideoBuffer_width(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->mWidth;
    }
    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_VideoBuffer_height(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->mHeight;
    }
    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_VideoBuffer_duration(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->mDuratioin;
    }
    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_VideoBuffer_stride(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->mStride;
    }
    return 0;
}extern "C"
JNIEXPORT jbyteArray JNICALL
Java_com_media_gankers_medianative_VideoBuffer_dump(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        jbyteArray array = env->NewByteArray(nativeBuffer->size());
        env->SetByteArrayRegion(array, 0, nativeBuffer->size(), (jbyte *)nativeBuffer->data());
        return array;
    }
    return nullptr;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_VideoBuffer_copyToJava(JNIEnv *env, jobject instance,
                                                          jobject buffer) {

    // TODO

    return 0;
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_media_gankers_medianative_VideoBuffer_create(JNIEnv *env, jclass type, jint w, jint h,
                                                      jint timestamp, jint duration, jint stride,
                                                      jint size) {

    // TODO
    VideoBuffer *buffer = new VideoBuffer();
    buffer->allocMem(size);
    buffer->mStride = stride;
    buffer->mWidth = w;
    buffer->mHeight = h;
    buffer->mTimestamp = timestamp;
    buffer->mDuratioin = duration;
    jmethodID mid = env->GetMethodID(type, "<init>", "(J)V");
    int64_t tb = (int64_t)buffer;
    jobject jniDemo = env->NewObject(type, mid, tb);
    return jniDemo;
}extern "C"
JNIEXPORT void JNICALL
Java_com_media_gankers_medianative_Buffer_addRef(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    Buffer *nativeBuffer = nullptr;
    nativeBuffer = (Buffer *)env->GetLongField(instance, nativeField);

    if (nativeBuffer) {
        nativeBuffer->addRef();
    }
}extern "C"
JNIEXPORT void JNICALL
Java_com_media_gankers_medianative_Buffer_decRef(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    Buffer *nativeBuffer = nullptr;
    nativeBuffer = (Buffer *)env->GetLongField(instance, nativeField);

    if (nativeBuffer) {
        nativeBuffer->decRef();
    }
}extern "C"
JNIEXPORT jlong JNICALL
Java_com_media_gankers_medianative_VideoBuffer_timestamp(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->mTimestamp;
    }
    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_TextureBuffer_textureId(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    TexBuffer *nativeBuffer = nullptr;
    nativeBuffer = TexBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->texId();
    }

    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_TextureBuffer_width(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    TexBuffer *nativeBuffer = nullptr;
    nativeBuffer = TexBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->width();
    }

    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_TextureBuffer_height(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    TexBuffer *nativeBuffer = nullptr;
    nativeBuffer = TexBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->height();
    }

    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_TextureBuffer_textureType(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    TexBuffer *nativeBuffer = nullptr;
    nativeBuffer = TexBuffer::get((Buffer *)env->GetLongField(instance, nativeField));
    if (nativeBuffer) {
        return nativeBuffer->texType();
    }

    return 0;
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_media_gankers_medianative_TextureBuffer_create(JNIEnv *env, jclass type, jint w, jint h,
                                                        jbyteArray bytes_) {
    jbyte *bytes = env->GetByteArrayElements(bytes_, NULL);

    // TODO
    TexBuffer *buffer = new TexBuffer();
    buffer->addRef();

    if (!buffer->createTexture(w ,h, bytes)) {
        buffer->decRef();
        buffer = nullptr;
    }

    jobject jniDemo = nullptr;
    if (buffer) {
        jmethodID mid = env->GetMethodID(type, "<init>", "(J)V");
        int64_t tb = (int64_t) buffer;
        jniDemo = env->NewObject(type, mid, tb);
    }

    buffer->decRef();
    env->ReleaseByteArrayElements(bytes_, bytes, 0);
    return jniDemo;
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_media_gankers_medianative_VideoBuffer_get(JNIEnv *env, jclass type, jobject buffer) {

    // TODO
    jclass cls = env->GetObjectClass(buffer);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    VideoBuffer *nativeBuffer = VideoBuffer::get((Buffer *)env->GetLongField(buffer, nativeField));

    jobject obj = nullptr;
    if (nativeBuffer) {
        jmethodID mid = env->GetMethodID(type, "<init>", "(J)V");
        int64_t tb = (int64_t)nativeBuffer;
        obj = env->NewObject(type, mid, tb);
    }
    return obj;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_ISource_start(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    int ret = ERRNUM();
    if (source) {
        ret = source->start();
    }
    return ret;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_ISource_stop(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    int ret = ERRNUM();
    if (source) {
        ret = source->stop();
    }
    return ret;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_ISource_release(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    int ret = ERRNUM();
    if (source) {
        ret = source->release();
        env->SetLongField(instance,nativeField, 0);
        delete source;
    }
    return ret;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_ISource_pause(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    int ret = ERRNUM();
    if (source) {
        ret = source->pause();
    }
    return ret;

}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_ISource_prepare(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    int ret = ERRNUM();
    if (source) {
        ret = source->prepare();
    }
    return ret;
}extern "C"
JNIEXPORT jboolean JNICALL
Java_com_media_gankers_medianative_ISource_loop(JNIEnv *env, jobject instance) {

    SCOPEDDEBUG();
    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");
    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    jmethodID onMessageMethodId = env->GetMethodID(cls, "onMessage", "(IIILjava/lang/Object;)V");

    while (source) {
        ALOGI("Start MediaSource thread on native ...");

        MediaSource::Message msg;
        if (!source->getNotify(msg)) {
            break;
        }

        env->CallVoidMethod(instance, onMessageMethodId, msg.what, msg.arg0, msg.arg1, msg.obj);
    }
    ALOGI("Exit MediaSource thread on native done");
    return false;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_CameraMediaSource_initNative(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");

    CameraMediaSource *source = new CameraMediaSourceOnAndroid;
    source->needMsgQueue();
    env->SetLongField(instance, nativeField, (long) source);
    return 0;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_FileSource_initNative(JNIEnv *env, jobject instance) {

    // TODO
    return ERRNUM();
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_FileSource_config(JNIEnv *env, jobject instance, jint key,
                                                     jobject obj) {

    // TODO
    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");

    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    int ret = ERRNUM();
    if (!source) {
        return ret;
    }

    switch (key) {
        case kKeyFrameRate:
        case kKeyBitRate:
        case kKeyWidth:
        case kKeyHeight:
        {
            int value;
            if (JNIHelper::asInt(env, obj, value)) {
                ret = source->setParameter(key, &value);
            }
            break;
        }
        default:
            ret = ERRNUM();
    }
    return ret;
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_CameraMediaSource_config(JNIEnv *env, jobject instance, jint key,
                                                            jobject obj) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");

    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    int ret = ERRNUM();
    if (!source) {
        return ret;
    }

    switch (key) {
        case kKeyFrameRate:
        case kKeyBitRate:
        case kKeyWidth:
        case kKeyHeight:
        case CameraMediaSourceOnAndroid::kKeyCamSourceFront:
        case CameraMediaSourceOnAndroid::kKeyCamSourceMirror:

        {
            int value;
            if (JNIHelper::asInt(env, obj, value)) {
                ret = source->setParameter(key, &value);
            }
            break;
        }
        default:
            ret = ERRNUM();
    }
    return ret;
}extern "C"
JNIEXPORT void JNICALL
Java_com_media_gankers_medianative_CameraWrapper_nativeFrameAvailable(JNIEnv *env, jobject instance,
                                                                      jlong ts,
                                                                      jfloatArray matrix_) {
    SCOPEDDEBUG();
    jfloat *matrix = env->GetFloatArrayElements(matrix_, NULL);

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");

    CameraMediaSourceOnAndroid *source = (CameraMediaSourceOnAndroid *) (env->GetLongField(instance, nativeField));

    if (source) {
        source->onFrameAvailable(ts, matrix);
    }

    env->ReleaseFloatArrayElements(matrix_, matrix, 0);
}extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_ISource_status(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeObject", "J");

    MediaSource *source = (MediaSource *) (env->GetLongField(instance, nativeField));

    if (source) {
        return source->status();
    }
    return ERRNUM();
}