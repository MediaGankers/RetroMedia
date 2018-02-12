#define LOG_TAG "media-native.cpp"

#include <jni.h>
#include <string>
#include <assert.h>
#include "old/VideoSource.h"
#include "old/VideoBufferWrapper.h"
#include "log/utils_log.h"
#include "engine/MediaEngine.h"

extern "C" JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    ALOGD("JNI_OnLoad!");
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        ALOGE("ERROR: GetEnv failed\n");
        return -1;
    }
    assert(env != NULL);
    MediaEngine::inst().initInternal(vm, env);
    /* success -- return valid version number */
    return JNI_VERSION_1_6;
}

extern "C" JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    ALOGD("JNI_OnUnload!");
}

extern "C" JNIEXPORT void JNICALL
Java_com_media_gankers_medianative_MediaNative_init(JNIEnv *env, jobject obj) {
    MediaEngine::inst().init(env, obj);
}

extern "C" JNIEXPORT void JNICALL
Java_com_media_gankers_medianative_MediaNative_startWork(JNIEnv *env, jobject obj) {
    MediaEngine::inst().startWork();
}

extern "C" JNIEXPORT void JNICALL
Java_com_media_gankers_medianative_MediaNative_stopWork(JNIEnv *env, jobject obj) {
    MediaEngine::inst().stopWork();
}