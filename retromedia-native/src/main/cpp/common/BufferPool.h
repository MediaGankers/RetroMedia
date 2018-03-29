//
// Created by chao on 2018/3/11.
//

#ifndef LIVETEST_BufferPool_H
#define LIVETEST_BufferPool_H

#include <list>
#include <mutex>
#include <string>
#include <condition_variable>
#include "Buffer.h"

class BufferPool: public Buffer::BufferRecycler{
typedef  std::list<Buffer *> BufferList;

public:
    class BufferOnBufferPool: public Buffer{
        friend class BufferPool;
    public:
        BufferOnBufferPool(BufferRecycler *r);

    protected:
        void resetRecycler(BufferRecycler *r);

        ~BufferOnBufferPool();
    };

protected:
    int mSize;
    int mMaxSize;
    std::string mName;
    BufferList mEmptyQueue;
    std::mutex mEmptyQueueLock;
    std::condition_variable mEmptyQueueCond;

    BufferList mQueue;
    std::mutex mQueueLock;

private:
   virtual void release(Buffer *buffer);

protected:
    virtual Buffer *createBuffer();

public:
    BufferPool(int maxNum, int bufferSize, const std::string &name);

    virtual int bufferSize();

    virtual int count();

    virtual int emptyBufferCount();

    // Block call, return AVBuffer instence with reference is 1.
    virtual Buffer *poll();

    // Block call, return AVBuffer instence with reference is 1.
    virtual Buffer *pollTime(long waitMs = 0);

    //NoBlock call, return AVBuffer instence with reference is 1.
    virtual Buffer *peek();

    const char * name();

    virtual ~BufferPool();
};


#endif //LIVETEST_BufferPool_H
