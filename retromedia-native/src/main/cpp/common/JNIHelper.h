//
// Created by chao on 2018/3/26.
//

#ifndef RETROMEDIA_JNIHELPER_H
#define RETROMEDIA_JNIHELPER_H

#include <jni.h>
#include <stdint.h>

class JNIHelper {
public:
    class GlobalObject {
    public:
        GlobalObject(jobject obj = nullptr);
        void setObj(jobject obj);
        ~GlobalObject();

    public:
        jobject mObj;
    };

    class LocalRefObject {
    public:
        LocalRefObject(jobject obj);
        void setObj(jobject obj);
        ~LocalRefObject();

    public:
        jobject mObj;
    };

public:
    JNIHelper();

    static bool asInt(JNIEnv *env, jobject obj, int &value);

    static bool asInt64(JNIEnv *env, jobject obj, int64_t &value);

    static JavaVM *getJVM();

    static void setupJVM(JavaVM *jvm);

    JNIEnv *env();

    ~JNIHelper();

private:
    bool mAttach;
};


#endif //RETROMEDIA_JNIHELPER_H
