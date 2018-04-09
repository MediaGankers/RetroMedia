//
// Created by chao on 2018/3/29.
//

#include <assert.h>
#include <common/DebugHelper.h>
#include "JavaDeliver.h"
#include "j4a/buffer.h"
#include "j4a/i_deliver.h"

JavaDeliver::JavaDeliver(jobject obj):mObj(obj) {
    SCOPEDDEBUG();
}

int JavaDeliver::deliver(Buffer *buffer, StreamType type) {
    JNIHelper helper;
    jobject jbuffer = J4AC_com_media_gankers_medianative_Buffer__Buffer(helper.env(), (int64_t)buffer);
    
    if (jbuffer) {
        J4AC_com_media_gankers_medianative_IDeliver__deliver(helper.env(), mObj.mObj, jbuffer, (jint)type);

     JNIHelper::LocalRefObject l(jbuffer);
    }
    return 0;
}

JavaDeliver::~JavaDeliver() {
SCOPEDDEBUG();
}

int JavaDeliver::decRef() {
    int cnt = Reference::decRef();
    assert(cnt >= 1);
    if (cnt == 1) {
        delete this;
    }
    return 0;
}
