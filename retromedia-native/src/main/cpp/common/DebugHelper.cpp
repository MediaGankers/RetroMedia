//
// Created by chao on 2018/3/27.
//
#define LOG_TAG "DebugHelper"
#include "DebugHelper.h"

#ifdef DEBUG

#include "log_defs.h"
DebugHelper::DebugHelper(const char *file, const char *name, int line) {
    mStartTime = getCurrentTime();
    mFile = file;
    int idx = mFile.rfind("/");

    if (idx >=0 ) {
        mFile = file + idx + 1;
    }

    mName = name;
    mLine = line;

    ALOGI("%s:%s:%d --> ", mFile.c_str(), name, line);
}


DebugHelper::~DebugHelper() {
   struct timeval tv;
   gettimeofday(&tv,NULL);

   int64_t time = ((uint64_t)tv.tv_sec) * 1000 + ((uint64_t)tv.tv_usec) / 1000;

    ALOGI("%s:%s:%d [%d ms]<-- ", mFile.c_str(), mName.c_str(), mLine, (int)(getCurrentTime() - mStartTime));
}
#endif