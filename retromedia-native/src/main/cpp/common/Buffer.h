//
// Created by chao on 2018/3/22.
//

#ifndef RETROMEDIA_BUFFER_H
#define RETROMEDIA_BUFFER_H


#include "Reference.h"

class Buffer: public Reference {

public:
    class BufferRecycler {
    public:
        virtual void release(Buffer *buffer);
    };

public:
    Buffer(BufferRecycler *recycler = nullptr);

    virtual int addRef();

    virtual int decRef();

    virtual int type()const;

    virtual int allocMem(int size);

    virtual uint8_t *data();
    const uint8_t *data()const;

    int size();

protected:
    virtual void release(Buffer *);

    virtual uint8_t *alloc(int size);

    virtual void dealloc(void *p, int size = 0);

protected:
    virtual ~Buffer();

protected:
    BufferRecycler *mRecycler;
    uint8_t *mData;
    int mSize;
};


#endif //RETROMEDIA_BUFFER_H
