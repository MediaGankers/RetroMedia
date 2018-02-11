//
// Created by 罗明川 on 2/11/18.
//

#ifndef RETROMEDIA_MEDIAENGINE_H
#define RETROMEDIA_MEDIAENGINE_H

#include <jni.h>
#include <list>
#include <thread>

class MediaEngine {
private:
    std::list<jobject> mCallbacks;
    jmethodID mOnData;
    bool mFlag = true;
    std::shared_ptr<std::thread> mThread;
    JavaVM *javaVM;
    JNIEnv *jniEnv;

    MediaEngine();

    ~MediaEngine();

public:
    static MediaEngine &inst() {
        static MediaEngine sInstance;
        return sInstance;
    }

    bool initInternal(JavaVM *jvm, JNIEnv *);

    void init(JNIEnv*, jobject);

    void startWork();

    void stopWork();

    void realJob();
};


#endif //RETROMEDIA_MEDIAENGINE_H
