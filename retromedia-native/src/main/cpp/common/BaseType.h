//
// Created by chao on 2018/2/6.
//

#ifndef RETROMEDIA_VIDEOTYPE_H
#define RETROMEDIA_VIDEOTYPE_H
#include <inttypes.h>
#include <unistd.h>
#include "Buffer.h"

#define ERRNUM() (-__LINE__)

enum VideoPictureFormat {
    kVideoPictureFormatUnknown = 0,
    kVideoPictureFormatI420,
};

enum StreamType {
    kStreamUnknown,
    kStreamVideo,
    kStreamAudio
};

class IDeliver: public Reference{
public:
    virtual int deliver(Buffer *buffer, StreamType type) = 0;
};

#endif //RETROMEDIA_VIDEOTYPE_H
