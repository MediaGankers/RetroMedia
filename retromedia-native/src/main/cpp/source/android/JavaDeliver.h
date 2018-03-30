//
// Created by chao on 2018/3/29.
//

#ifndef RETROMEDIA_JAVADELIVER_H
#define RETROMEDIA_JAVADELIVER_H


#include <common/BaseType.h>
#include <jni.h>
#include <common/JNIHelper.h>

class JavaDeliver: public IDeliver {
private:
    JNIHelper::GlobalObject mObj;
public:
    JavaDeliver(jobject obj);

    virtual int deliver(Buffer *buffer, StreamType type);

    virtual int decRef();

protected:
    ~JavaDeliver();

};


#endif //RETROMEDIA_JAVADELIVER_H
