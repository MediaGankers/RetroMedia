//
// Created by chao on 2018/3/26.
//

#include "JNIHelper.h"

static JavaVM *sJVM;

bool JNIHelper::asInt(JNIEnv *env, jobject obj, int &value) {
    JNIHelper jniHelper;
    bool ret = false;

    if (!obj) {
        goto exit;
    }

    if (!env) {
        env = jniHelper.env();
    }

    if (!env) {
        goto exit;
    }

    static jclass intClass = env->FindClass("java/lang/Integer");
    static jmethodID intMethod = intClass ? env->GetMethodID(intClass, "intValue", "()I") : nullptr;

    if (intMethod) {
        value = env->CallIntMethod(obj, intMethod);
        ret = true;
    }

    exit:
    return ret;
}

bool JNIHelper::asInt64(JNIEnv *env, jobject obj, int64_t &value) {
    JNIHelper jniHelper;
    bool ret = false;

    if (!obj) {
        goto exit;
    }

    if (!env) {
        env = jniHelper.env();
    }

    if (!env) {
        goto exit;
    }

    static jclass intClass = env->FindClass("java/lang/Long");
    static jmethodID longMethod = intClass ? env->GetMethodID(intClass, "longValue", "()J") : nullptr;

    if (longMethod) {
        value = env->CallLongMethod(obj, longMethod);
        ret = true;
    }

    exit:
    return ret;
}

JavaVM *JNIHelper::getJVM() {
    return sJVM;
}

void JNIHelper::setupJVM(JavaVM *jvm) {
    sJVM = jvm;
}

JNIHelper::JNIHelper() {
    mAttach = false;
}

JNIEnv *JNIHelper::env() {
    JNIEnv *env = nullptr;
    if (sJVM) {
        sJVM->GetEnv((void **)&env, JNI_VERSION_1_6);
        if (env == nullptr) {
            sJVM->AttachCurrentThread(&env, nullptr);
            mAttach = true;
        }
    }
	return env;
}

JNIHelper::~JNIHelper() {
    if (mAttach) {
        sJVM->DetachCurrentThread();
        mAttach = false;
    }
}

JNIHelper::GlobalObject::GlobalObject(jobject obj) {
    mObj = nullptr;
    setObj(obj);
}

JNIHelper::GlobalObject::~GlobalObject() {
    setObj(nullptr);
}

void JNIHelper::GlobalObject::setObj(jobject obj) {
    JNIHelper helper;
    if (mObj) {
        helper.env()->DeleteGlobalRef(mObj);
        mObj = nullptr;
    }

    if (obj) {
        mObj = helper.env()->NewGlobalRef(obj);
    }
}

JNIHelper::LocalRefObject::LocalRefObject(jobject obj) {
    mObj = nullptr;
    setObj(obj);
}

JNIHelper::LocalRefObject::~LocalRefObject() {
    setObj(nullptr);
}

void JNIHelper::LocalRefObject::setObj(jobject obj) {
    JNIHelper helper;
    if (mObj) {
        helper.env()->DeleteLocalRef(mObj);
        mObj = nullptr;
    }

    if (obj) {
        mObj = helper.env()->NewLocalRef(obj);
    }
}
