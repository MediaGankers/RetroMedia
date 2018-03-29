//
// Created by chao on 2018/3/22.
//
#define LOG_TAG "Buffer"
#include "log_defs.h"

#include <assert.h>
#include <string.h>
#include "Buffer.h"

#define BUFFER_TYPE (int)('b' << 24 | 'u' << 16 | 'f' << 8 | 'f')

static std::atomic<int> sMemTestCnt;
int Buffer::addRef() {
    return Reference::addRef();
}

int Buffer::decRef() {
    int cnt = Reference::decRef();
    assert(cnt >= 1);
    if (cnt == 1) {
        mRecycler->release(this);
    }
    return cnt - 1;
}

void Buffer::release(Buffer *buffer) {
    delete buffer;
}

Buffer::Buffer(BufferRecycler *recycler) {
    static BufferRecycler defaultRecycler;
    mRecycler = recycler ? recycler : &defaultRecycler;
    mData = nullptr;
    mSize = 0;
}

Buffer::~Buffer() {
    if (mData) {
        dealloc(mData, mSize);
        mData = nullptr;
        mSize = 0;
    }
}

uint8_t * Buffer::alloc(int size) {
    uint8_t *buffer = nullptr;
    if (size > 0) {
        buffer = (uint8_t *)malloc(size);
        sMemTestCnt += size;
    }

    ALOGD("======test alloc mem %d buffer %p=======", (int)sMemTestCnt, buffer);
    return buffer;
}

void Buffer::dealloc(void *p, int size) {
    if (p) {
        sMemTestCnt -= size;
        free(p);
    }
    ALOGD("======test dealloc mem %d=======", (int)sMemTestCnt);
}

int Buffer::type() const {
    return BUFFER_TYPE;
}

uint8_t *Buffer::data() {
    return mData;
}

int Buffer::size() {
    return mSize;
}

int Buffer::allocMem(int size) {
    mData = alloc(size);
    mSize = size;
    ALOGD("mData %p mSize %d size %d", mData, mSize, size);
    return mSize;
}

void Buffer::BufferRecycler::release(Buffer *buffer) {
    delete buffer;
}
