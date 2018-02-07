#include <jni.h>
#include <string>
#include "VideoSource.h"
#include "VideoBufferWrapper.h"

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_media_gankers_medianative_MediaNative_helloFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from JNI, let's make it awesome";
    IVideoSource *source = VideoSource::create(kVideoSourceFile);
    source->setSource("/sdcard/BasketballDrill_832x480_50.yuv");
    source->setSourceInfo(832, 480, kVideoPictureFormatI420);
    source->open();
    source->start();
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_media_gankers_medianative_VideoBuffer_size(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeHandle", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = (VideoBuffer *)env->GetLongField(instance, nativeField);
    if (nativeBuffer) {
        return nativeBuffer->size();
    }
    return 0;

}extern "C"
JNIEXPORT jobject JNICALL
Java_com_media_gankers_medianative_VideoBuffer_byteBuffer(JNIEnv *env, jobject instance) {

    // TODO
    return nullptr;
}extern "C"
JNIEXPORT jobject JNICALL
Java_com_media_gankers_medianative_VideoBuffer_create(JNIEnv *env, jclass type, jint size) {

    // TODO
    jobject obj = nullptr;
    if (size <=0 ) {
        return obj;
    }

    VideoBuffer *buffer = VideoBufferWrapper::create();
    if (buffer) {
        if (size != buffer->alloc(size)) {
            buffer->release();
            return obj;
        }
    }

    jmethodID ctor = env->GetMethodID(type, "<init>", "()V");
    obj = env->NewObject(type, ctor);
    if (obj != nullptr) {
        jfieldID nativeField = env->GetFieldID(type, "mNativeHandle", "J");
        env->SetLongField(obj, nativeField, (int64_t)buffer);
    }

    return obj;
}extern "C"
JNIEXPORT void JNICALL
Java_com_media_gankers_medianative_VideoBuffer_release(JNIEnv *env, jobject instance) {

    // TODO
    jclass cls = env->GetObjectClass(instance);
    jfieldID nativeField = env->GetFieldID(cls, "mNativeHandle", "J");
    VideoBuffer *nativeBuffer = nullptr;
    nativeBuffer = (VideoBuffer *)env->GetLongField(instance, nativeField);

    env->SetLongField(instance, nativeField, 0);
    if (nativeBuffer) {
        nativeBuffer->release();
    }
}