//
// Created by chao on 2018/2/6.
//
#define LOG_TAG "JniBridge.cpp"

#include "log_defs.h"

#include "JniBridge.h"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    ALOGD("JNI_OnLoad!");
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    ALOGD("JNI_OnUnload!");
}