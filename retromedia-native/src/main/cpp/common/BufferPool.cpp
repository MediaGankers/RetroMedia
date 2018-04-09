//
// Created by chao on 2018/3/11.
//
#define LOG_TAG "BufferPool"

#include<algorithm>
#include "BufferPool.h"
#include "log_defs.h"

BufferPool::BufferPool(int maxNum, int bufferSize, const std::string &name) {
    mSize = bufferSize;
    mMaxSize = maxNum;
    mName = name;
    mEmptyQueue.clear();
    mQueue.clear();
}

int BufferPool::bufferSize() {
    return mSize;
}

int BufferPool::emptyBufferCount() {
    std::unique_lock<std::mutex> lock(mQueueLock);
    std::unique_lock<std::mutex> lock1(mEmptyQueueLock);
    return (mMaxSize - mQueue.size() + mEmptyQueue.size());
}

BufferPool::~BufferPool() {
  std::unique_lock<std::mutex> lock1(mEmptyQueueLock);
  std::unique_lock<std::mutex> lock(mQueueLock);
    mEmptyQueueCond.notify_all();

    int totalCount = mQueue.size();
    int emptyCount = mEmptyQueue.size();

    if (totalCount > 0 && totalCount != emptyCount) {
        ALOGE("BufferPool[%s] no recycler all buffer!!", mName.c_str());
    }

    while (!mQueue.empty()) {
        Buffer *buffer = mQueue.front();
        mQueue.pop_front();
        BufferList::iterator it = std::find(mEmptyQueue.begin(), mEmptyQueue.end(), buffer);

        BufferOnBufferPool * bufferOnQueue = (BufferOnBufferPool *)buffer;
        bool laterRelease = false;

        // not in empty queue
        if (it == mEmptyQueue.end()) {
            static Buffer::BufferRecycler defaultRecycler;
            laterRelease = true;
            //No safe, But we don't want to happen the memory leak.
            bufferOnQueue->resetRecycler(&defaultRecycler);
        }

        ALOGI("Release Buffer %p [%s]", bufferOnQueue, laterRelease ? "LaterRelease" : "Done");
        if (!laterRelease) {
            delete bufferOnQueue;
        }
    }
}

Buffer *BufferPool::poll() {
    return pollTime(LONG_MAX);
}

Buffer *BufferPool::peek() {
    return pollTime(0);
}

Buffer *BufferPool::pollTime(long waitMs) {
    Buffer *buffer = nullptr;
    {
        std::unique_lock<std::mutex> lock(mEmptyQueueLock);
        if (!mEmptyQueue.empty()) {
            buffer = mEmptyQueue.front();
            mEmptyQueue.pop_front();
        }
    }

    if (!buffer) {
        std::unique_lock<std::mutex> lock(mQueueLock);

        int allocCount = mQueue.size();
        if (allocCount < mMaxSize) {
            buffer = createBuffer();

            if (!buffer) {
                ALOGW("Alloc Buffer failed on [%s]", mName.c_str());
            }else {
                mQueue.push_back(buffer);
            }
        }
    }

    if (!buffer && waitMs > 0) {
        std::unique_lock<std::mutex> lock(mEmptyQueueLock);
        if (!mEmptyQueue.empty()) {
            buffer = mEmptyQueue.front();
            mEmptyQueue.pop_front();
        }else {
            mEmptyQueueCond.wait_for(lock, std::chrono::duration<int64_t, std::milli>(waitMs));
            if(!mEmptyQueue.empty()) {
                buffer = mEmptyQueue.front();
                mEmptyQueue.pop_front();
            }
        }
    }

    if (buffer) {
        buffer->addRef();
    }

    return buffer;
}

void BufferPool::release(Buffer *buffer) {
    std::lock_guard<std::mutex> lock(mEmptyQueueLock);
    mEmptyQueue.push_back(buffer);
    mEmptyQueueCond.notify_one();
}

int BufferPool::count() {
    return mMaxSize;
}

Buffer *BufferPool::createBuffer() {
    BufferOnBufferPool *buffer = new BufferOnBufferPool(this);
    if (buffer->allocMem(mSize) != mSize) {
        delete buffer;
        buffer = nullptr;
    }
    return buffer;
}

const char *BufferPool::name() {
    return mName.c_str();
}

BufferPool::BufferOnBufferPool::BufferOnBufferPool(Buffer::BufferRecycler *r) : Buffer(
        r) {

}

void BufferPool::BufferOnBufferPool::resetRecycler(Buffer::BufferRecycler *r) {
    mRecycler = r;
}

BufferPool::BufferOnBufferPool::~BufferOnBufferPool() {
    ALOGI("~Dtor Buffer %p", this);
}
