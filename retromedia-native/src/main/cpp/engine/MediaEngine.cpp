//
// Created by 罗明川 on 2/11/18.
//

#define LOG_TAG "MediaEngine.cpp"

#include "MediaEngine.h"
#include "../log/utils_log.h"
#include "jni.h"
#include "../macro/macro.h"
#include <thread>
#include <unistd.h>

bool MediaEngine::initInternal(JavaVM *jvm, JNIEnv *env) {
    ALOGD("initialize");
    javaVM = jvm;
    jniEnv = env;
    CHECK_POINTER(jniEnv, FALSE, "jniEnv is NULL!");

    //注意这里千万不能错
    jclass hostClazz = jniEnv->FindClass("com/media/gankers/medianative/MediaNative");
    CHECK_POINTER(hostClazz, FALSE, "can't execute FindClass!");

    GET_METHOD_ID(jniEnv, hostClazz, mOnData, "onData", "(Ljava/lang/String;)V");
    // get JVM object
    if (JNI_OK != jniEnv->GetJavaVM(&jvm)) {
        ALOGE("can't execute GetJavaVM!");
        return FALSE;
    }
    return false;
}

void MediaEngine::init(JNIEnv *env, jobject obj) {
    mNativeHandle = env->NewGlobalRef(obj);
}

void MediaEngine::startWork() {
    if (mRun == FALSE && mThreadRunning == FALSE) {
        mThread = std::shared_ptr<std::thread>(new std::thread(&MediaEngine::realJob, this));
    }
    waitThread();
}

void MediaEngine::stopWork() {
    mRun = FALSE;
}

void MediaEngine::realJob() {
    javaVM->AttachCurrentThread(&jniEnv, NULL);
    mRun = TRUE;
    while (mRun) {
        jniEnv->CallVoidMethod(mNativeHandle, mOnData, jniEnv->NewStringUTF(retValue.c_str()));
        usleep(1000 * 5000);
    }
    mThreadRunning = FALSE;
    javaVM->DetachCurrentThread();
}

void MediaEngine::waitThread() {
    while (!mRun) {
        usleep(1000 * 10);
    }
}

MediaEngine::MediaEngine() {
    retValue = "hello from jni callback.";
}

MediaEngine::~MediaEngine() {

}
