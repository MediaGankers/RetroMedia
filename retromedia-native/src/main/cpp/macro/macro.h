//
// Created by 罗明川 on 2/11/18.
//

#ifndef RETROMEDIA_MACRO_H
#define RETROMEDIA_MACRO_H

#include "../macro/macro.h"
#define TRUE true
#define FALSE false

#define CHECK_POINTER(pValue, rValue, ...) if(NULL == pValue) { ALOGE(__VA_ARGS__); return rValue; }

#define GET_METHOD_ID(jniEnv, hostClazz, var, func, spec);                        \
    var = jniEnv->GetMethodID(hostClazz, func, spec);    \
    CHECK_POINTER(var, FALSE, "can't execute GetMethodID of %s !", func);

#endif //RETROMEDIA_MACRO_H
