//
// Created by chao on 2018/2/6.
//

#ifndef RETROMEDIA_VIDEOTYPE_H
#define RETROMEDIA_VIDEOTYPE_H
#include <inttypes.h>
#include <unistd.h>

#define ERRNUM() (-__LINE__)
#define LOGD(fmt, ...)


enum VideoSourceType{
    kVideoSourceUnknown = 0,
    kVideoSourceFile,
    kVideoSourceCamera
};

enum VideoPictureFormat {
    kVideoPictureFormatUnknown = 0,
    kVideoPictureFormatI420,
};

class VideoBuffer{
public:
    virtual int alloc(uint32_t ) = 0;
    virtual int relloc(uint32_t ) = 0;
    virtual uint8_t *date() = 0;
    virtual int size() = 0;
    virtual int copyTo(VideoBuffer *) = 0;
    virtual void release() = 0;
protected:
    virtual ~VideoBuffer(){}
};

class IDeliver {
public:
    virtual int deliver(VideoBuffer &buffer) = 0;
};

#endif //RETROMEDIA_VIDEOTYPE_H
