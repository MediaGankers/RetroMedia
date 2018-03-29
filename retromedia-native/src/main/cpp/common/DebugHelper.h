//
// Created by chao on 2018/3/27.
//

#ifndef RETROMEDIA_DEBUGHELPER_H
#define RETROMEDIA_DEBUGHELPER_H

#include <libyuv/basic_types.h>
#include <sys/time.h>
#include <linux/time.h>

#ifdef DEBUG
#include <string>

class DebugHelper {
    std::string mFile;
    std::string mName;
    int mLine;
    int64_t mStartTime;
public:
    DebugHelper(const char *file, const char *name, int line);
    ~DebugHelper();
};

#define SCOPEDDEBUG() DebugHelper __debug__(__FILE__, __FUNCTION__, __LINE__)
#else
#define SCOPEDDEBUG()
#endif

inline int64_t getCurrentTime() {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return ((uint64_t)tv.tv_sec) * 1000 + ((uint64_t)tv.tv_usec) / 1000;
}
#endif //RETROMEDIA_DEBUGHELPER_H
