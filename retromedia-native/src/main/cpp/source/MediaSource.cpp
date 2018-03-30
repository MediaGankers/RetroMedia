//
// Created by chao on 2018/3/25.
//
#define LOG_TAG "MediaSource"

#include <BaseType.h>
#include <log_defs.h>
#include <common/DebugHelper.h>
#include <common/TexBufferPool.h>
#include <common/VideoBufferPool.h>
#include <Decoder.h>
#include "source/MediaSource.h"

MediaSource::MediaSource() {
    mStatus = kIdle;
    mMetaData = new MetaData;
    mMetaData->addRef();
    mDelivers.clear();
}

int MediaSource::start() {
    setStatus(kStarted);
    return 0;
}

int MediaSource::stop() {
    setStatus(kStoped);
    return 0;
}

int MediaSource::pause() {
    setStatus(kPause);
    return 0;
}

int MediaSource::prepare() {
    setStatus(kPreared);
    return 0;
}

int MediaSource::release() {
    setStatus(kEnd);
    return 0;
}

int MediaSource::reset() {
    clearDeliver();
    mMetaData->clear();
    setStatus(kIdle);
    return 0;
}

void MediaSource::addDeliver(IDeliver *d) {
    if (d) {
        d->addRef();
        mDelivers.push_back(d);
    }
}

int MediaSource::setParameter(int key, void *obj) {
    int ret = ERRNUM();
    switch (key) {
        case kKeyFrameRate:
        case kKeyBitRate:
        case kKeyWidth:
        case kKeyHeight:
            mMetaData->setInt32(key, *((int *)obj));
            ret = 0;
            break;
    }
    return ret;
}

MediaSource::Status MediaSource::status() {
    return mStatus;
}

MediaSource::Status MediaSource::setStatus(MediaSource::Status stat) {
        Status oldStatus = mStatus;
        mStatus = stat;
        ALOGI("Status convert %s - > %s", statusToString(oldStatus), statusToString(mStatus));
    mStatLockCond.notify_all();
    return status();
}

void MediaSource::removeDeliver(IDeliver *d) {
    if (d) {
        std::list<IDeliver *>::iterator it = std::find(mDelivers.begin(), mDelivers.end(), d);
        if (it != mDelivers.end()) {
            mDelivers.remove(d);
            d->decRef();
        }
    }
}

bool MediaSource::getNotify(MediaSource::Message &message) {
    return false;
}

bool MediaSource::deliver(Buffer *buffer, StreamType type) {
    SCOPEDDEBUG();

        TexBuffer *b = TexBuffer::get(buffer);
        VideoBuffer *vb = VideoBuffer::get(buffer);

    if (b) {
        ALOGI("Deliver a texture buffer ID %d w %d h %d Stream %d", b->texId(), b->width(), b->height(), type);
    }else if (vb ) {
        ALOGI("Deliver a Video buffer addr %p w %d h %d  Stream %d", vb->data(), vb->mWidth, vb->mHeight, type);
    }

    bool d = false;
    std::list<IDeliver *>::iterator it = mDelivers.begin();
    while (it != mDelivers.end()) {
        (*it)->deliver(buffer, type);
        d = true;
        it++;
    }
    return d;
}

MediaSource::~MediaSource() {
    clearDeliver();
    mMetaData->decRef();
}

void MediaSource::clearDeliver() {
    std::list<IDeliver *>::iterator it = mDelivers.begin();

    while (it != mDelivers.end()) {
        (*it)->decRef();
        mDelivers.pop_front();
        it = mDelivers.begin();
    }
}
