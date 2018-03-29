//
// Created by chao on 2018/3/23.
//

#include "VideoBufferPool.h"
#define VIDEOBUFFER_TYPE (int)('v' << 24 | 'd' << 16 | 'b' << 8 | 'f')

VideoBufferPool::VideoBufferPool(int maxNum, int bufferSize, const std::string &name) : BufferPool(
        maxNum, bufferSize, name) {}

Buffer *VideoBufferPool::createBuffer() {
    VideoBuffer *buffer = new VideoBuffer(this);
    if (buffer->allocMem(mSize) <= 0) {
        delete buffer;
        buffer = nullptr;
    }
    return buffer;
}

VideoBuffer *VideoBufferPool::pollVideoBuffer() {
    return (VideoBuffer *) BufferPool::poll();
}

VideoBuffer *VideoBufferPool::pollTimeVideoBuffer(long ms) {
    return (VideoBuffer *)BufferPool::pollTime(ms);
}

const VideoBuffer *VideoBuffer::get(const Buffer *buffer) {
    return buffer && buffer->type() == VIDEOBUFFER_TYPE ? (VideoBuffer *)buffer : nullptr;
}

VideoBuffer *VideoBuffer::get(Buffer * buffer) {
    return buffer && buffer->type() == VIDEOBUFFER_TYPE ? (VideoBuffer *)buffer : nullptr;
}

int VideoBuffer::type() const {
    return VIDEOBUFFER_TYPE;
}

VideoBuffer::VideoBuffer(Buffer::BufferRecycler *r) : BufferOnBufferPool(r) {
    mWidth = 0;
    mHeight = 0;
    mStride = 0;
    mColorType = -1;
    mTimestamp = 0;
    mDuratioin = 0;
}

VideoBuffer::~VideoBuffer() {

}

