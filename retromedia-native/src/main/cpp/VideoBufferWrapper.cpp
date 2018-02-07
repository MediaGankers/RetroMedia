//
// Created by chao on 2018/2/6.
//
#define LOG_TAG "VideoBufferWrapper.cpp"
#include "log_defs.h"

#include "VideoBufferWrapper.h"
#include <math.h>
#include <algorithm>

static std::atomic<int> sAllocMemSize(0);
class VideoBufferHeap: public VideoBuffer {
private:
    uint32_t mSize;
    uint8_t *mDate;
public:
    VideoBufferHeap() {
        mSize = 0;
        mDate = nullptr;
        ALOGD("Ctor VideoBufferHeap %p", this);
    }

    virtual int alloc(uint32_t size) {
        return relloc(size);
    }

    virtual int relloc(uint32_t size) {
        if (size != mSize && mSize > 0) {
            free(mDate);
            mSize = 0;
            mDate = nullptr;
        }

        if (size > 0) {
            mDate = (uint8_t *)malloc(size);
            memset(mDate, 0, size);
            mSize = size;
        }
        sAllocMemSize+= mSize;
        return mSize;
    }

    virtual uint8_t *date() {
        return mDate;
    }

    virtual int size() {
        return mSize;
    }

    virtual int copyTo(VideoBuffer *buffer) {
        uint8_t  copySize = 0;

        if (buffer && buffer->size() > 0) {
            copySize = std::min((int)buffer->size(), (int)mSize);
            memcpy(buffer->date(), mDate, copySize);
        }
        return copySize;
    }

    virtual void release() {
        //recycle buffer??
        if (mDate) {
            free(mDate);
            sAllocMemSize-=mSize;
        }
        delete this;
    }

    ~VideoBufferHeap() {
        ALOGD("Dtor VideoBufferHeap %p", this);
    }
};

VideoBuffer *VideoBufferWrapper::create() {
    return new VideoBufferHeap;
}

int VideoBufferWrapper::allocSize() {
    return sAllocMemSize;
}
