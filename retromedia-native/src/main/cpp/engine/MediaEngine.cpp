//
// Created by 罗明川 on 2/11/18.
//
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

    GET_METHOD_ID(jniEnv, hostClazz, mOnData, "onData", "([B)V");
    // get JVM object
    if (JNI_OK != jniEnv->GetJavaVM(&jvm)) {
        ALOGE("can't execute GetJavaVM!");
        return FALSE;
    }
    return false;
}

void MediaEngine::init(JNIEnv *env, jobject obj) {
    mCallbacks.push_back(env->NewGlobalRef(obj));
}

void MediaEngine::startWork() {
    mThread = std::auto_ptr<std::thread>(new std::thread(&MediaEngine::realJob, this));
    mThread->join();
}

void MediaEngine::stopWork() {
    mFlag = false;
}

void MediaEngine::realJob() {
    usleep(1000 * 10);
    while (mFlag) {
        std::list<jobject>::iterator iterator = mCallbacks.begin();
        while (iterator != mCallbacks.end()) {
            jniEnv->CallVoidMethod((*iterator), mOnData, "hello");
        }
    }
}

MediaEngine::MediaEngine() {

}

MediaEngine::~MediaEngine() {

}
