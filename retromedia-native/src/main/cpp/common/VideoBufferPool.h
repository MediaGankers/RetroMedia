//
// Created by chao on 2018/3/23.
//

#ifndef RETROMEDIA_VIDEOBUFFERPOOL_H
#define RETROMEDIA_VIDEOBUFFERPOOL_H


#include "BufferPool.h"

class VideoBuffer;
class VideoBufferPool: public BufferPool {
public:
    VideoBufferPool(int maxNum, int bufferSize, const std::string &name);

public:
    VideoBuffer *pollVideoBuffer();

    VideoBuffer *pollTimeVideoBuffer(long ms);

protected:
    Buffer *createBuffer();
};

class VideoBuffer: public BufferPool::BufferOnBufferPool {
    friend class VideoBufferPool;
public:
    VideoBuffer(BufferRecycler *r = nullptr);

    static const VideoBuffer *get(const Buffer *);

    static VideoBuffer *get(Buffer *);

    int type()const;

protected:
    ~VideoBuffer();

public:
    int mWidth;
    int mHeight;
    int mStride;
    int mColorType;
    int64_t mTimestamp;
    int64_t mDuratioin;
};

#endif //RETROMEDIA_VIDEOBUFFERPOOL_H
