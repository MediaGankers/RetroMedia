//
// Created by chao on 2018/3/22.
//

#ifndef RETROMEDIA_TEXBUFFERPOOL_H
#define RETROMEDIA_TEXBUFFERPOOL_H

#include "BufferPool.h"
#include "gl/GlUtils.h"

class TexBuffer;
class TexBufferPool: public BufferPool {
public:
    TexBufferPool(int maxNum, int w, int h, const std::string &name);

    inline TexBuffer *pollAsTexBuffer() {
        return (TexBuffer *)poll();
    }

    inline TexBuffer *pollAsTexBuffer(long ms) {
        return (TexBuffer *)pollTime(ms);
    }

protected:
    Buffer *createBuffer();

private:
    int mWidth;
    int mHeight;
    void *mData;
};

class TexBuffer: public BufferPool::BufferOnBufferPool{
    friend class TexBufferPool;
public:
    static const TexBuffer *get(const Buffer *);

    static TexBuffer *get(Buffer *);

    TexBuffer(BufferRecycler *r = nullptr);

    bool createTexture(int w, int h, void *data = NULL);

    int texId();

    int type()const;

    int width();

    int height();

    int texType();

protected:
    ~TexBuffer();
private:
    GLuint mTex;
    int mWidth;
    int mHeight;
    GLuint mTexType;
};

#endif //RETROMEDIA_TEXBUFFERPOOL_H
