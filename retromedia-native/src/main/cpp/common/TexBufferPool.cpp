//
// Created by chao on 2018/3/22.
//

#include "TexBufferPool.h"

TexBufferPool::TexBufferPool(int maxNum, int w, int h, const std::string &name) : BufferPool(
        maxNum, 0, name) {
    mWidth = w;
    mHeight = h;
    mData = NULL;
}

Buffer *TexBufferPool::createBuffer() {
    TexBuffer *buff = new TexBuffer(this);
    if (!buff->createTexture(mWidth, mHeight, mData) ) {
        delete buff;
        buff = NULL;
    }

    return buff;
}

#define TEXBUFFER_TYPE (int)('t' << 24 | 'e' << 16 | 'x' << 8 |'b' )

TexBuffer::TexBuffer(Buffer::BufferRecycler *r) : BufferOnBufferPool(r) {}


int TexBuffer::texId() {
    return mTex;
}

bool TexBuffer::createTexture(int w, int h, void *data) {
    mTex = GlUtils::createTexture2d(w, h, data);
    mWidth = w;
    mHeight = h;
    mTexType = GL_TEXTURE_2D;
    return mTex > 0;
}

int TexBuffer::type()const {
    return TEXBUFFER_TYPE;
}

const TexBuffer *TexBuffer::get(const Buffer *buffer) {
    return buffer && buffer->type() == TEXBUFFER_TYPE ? (TexBuffer *)buffer : NULL;
}

TexBuffer *TexBuffer::get(Buffer *buffer) {
    return buffer && buffer->type() == TEXBUFFER_TYPE ? (TexBuffer *)buffer : NULL;
}

TexBuffer::~TexBuffer() {
    if (mTex) {
        GlUtils::deleteTexture(1, &mTex);
    }
}

int TexBuffer::height() {
    return mHeight;
}

int TexBuffer::width() {
    return mWidth;
}

int TexBuffer::texType() {
    return mTexType;
}
