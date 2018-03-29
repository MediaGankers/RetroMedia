//
// Created by chao on 2018/3/11.
//

#define LOG_TAG "AVBufferTest"

#include "log_defs.h"

#include "VideoBufferPool.h"
#include "BufferPool.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <string>

#define DEBUG() printf("%s: %d\n", __FILE__,__LINE__);
int main(int size, char *args[]) {
    Buffer *buffer = new Buffer();
    DEBUG();
    buffer->addRef();
    DEBUG();

    int buffSize = buffer->allocMem(1024);
    DEBUG();
    assert(buffSize == buffer->size());
    DEBUG();
    memset(buffer->data(), 123, buffer->size());
    DEBUG();

    ALOGI("size %d  [100] = %d", buffSize, (int)buffer->data()[100]);
    assert(buffer->data()[100] == 123);
    assert(buffer->data()[200] == 123);
    assert(buffer->data()[1023] == 123);

    buffer->addRef();
    printf("ref cnt %d\n", buffer->count());

    buffer->decRef();
    buffer->decRef();
    DEBUG();


    BufferPool *queue = new BufferPool(3, 2000, "TestQueue");
    buffer = queue->poll();
    assert(queue->count() == 3);
    assert(queue->bufferSize() == 2000);
    assert(buffer->size() == 2000);

    assert(queue->emptyBufferCount() == 2);

    assert(buffer->count() == 1);
    buffer->addRef();
    assert(buffer->count() == 2);

    buffer->decRef();
    buffer->decRef();
    assert(queue->emptyBufferCount() == 3);

    delete queue;

    queue = new BufferPool(2, 3000, "TestQueue2");

    Buffer *b1 = queue->pollTime(1000);
    ALOGI("alloc buffer %p", b1);

    Buffer *b2 = queue->pollTime(1000);
    ALOGI("alloc buffer %p", b2);

    Buffer *b3 = queue->pollTime(1000);
    ALOGI("alloc buffer %p", b3);

    b3 = queue->pollTime(1000);
    ALOGI("alloc buffer %p", b3);

    b3 = queue->pollTime(1000);
    ALOGI("alloc buffer %p", b3);

    b3 = queue->peek();
    ALOGI("alloc buffer %p", b3);

    b3 = queue->pollTime(1000);
    ALOGI("alloc buffer %p", b3);

    b1->decRef();
    DEBUG();
    delete queue;
    DEBUG();

    b2->decRef();
    DEBUG();





    ALOGI("Test VideoBuffer");

    VideoBufferPool *vpoll = new VideoBufferPool(2, 800, "VBufferPollTest");

    VideoBuffer *buffer1 = vpoll->pollVideoBuffer();
    VideoBuffer *buffer2 = vpoll->pollVideoBuffer();

    VideoBuffer *buffer3 = vpoll->pollTimeVideoBuffer(1000);

    assert(buffer3 == nullptr);
    buffer1->decRef();
    buffer1 = vpoll->pollVideoBuffer();

    assert(buffer1 != nullptr);

    delete vpoll;

    buffer1->decRef();


    Buffer *b = buffer2;

    memset(b->data(), 111, b->size());
    assert(b->size() == 800);

    VideoBuffer *videoBuffer = VideoBuffer::get(b);

    assert(videoBuffer == buffer2);

    videoBuffer->mWidth = 1024;
    videoBuffer->mHeight = 1000;

    b->decRef();

    b = new Buffer;
    b->allocMem(200);
    b->addRef();

    videoBuffer = VideoBuffer::get(b);

    assert(videoBuffer == nullptr);

    b->decRef();
    return 0;
}