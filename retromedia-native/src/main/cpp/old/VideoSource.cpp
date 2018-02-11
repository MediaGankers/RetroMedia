//
// Created by chao on 2018/2/6.
//
#define LOG_TAG "VideoSource.cpp"

#include "../log/utils_log.h"
#include "VideoSource.h"
#include "VideoSourceFile.h"

#include <thread>

int VideoSource::setCallback(IDeliver *deliver) {
    mDelivers.push_back(deliver);
    return 0;
}

int VideoSource::removeCallback(IDeliver *deliver) {
    mDelivers.remove(deliver);
    return 0;
}

int VideoSource::start() {
    if (mThread == nullptr && !mRun) {
        mThread = std::auto_ptr<std::thread>(new std::thread(&VideoSource::readThread, this));
        waitThread();
        return 0;
    }
    return ERRNUM();
}

void VideoSource::readThread() {
    mRun = true;

    while (readDate() && mRun);

    ALOGD("read thread exit...");
}

void VideoSource::waitThread() {
    while (!mRun) {
        ALOGD("waitThread ...");
        usleep(1000 * 10);
    }
}

VideoSource::~VideoSource() {
    stop();
}

bool VideoSource::readDate() {
    ALOGD("readDate not implement.");
    return false;
}

VideoSource::VideoSource() {
    mRun = false;
    mThread = nullptr;
}

bool VideoSource::deliver(VideoBuffer *buffer) {
    bool ret = false;
    std::list<IDeliver *>::iterator item = mDelivers.begin();
    while (item != mDelivers.end()) {
        (*item)->deliver(*buffer);
        ret = true;
    }
    return ret;
}

int VideoSource::stop() {
    mRun = false;
    return 0;
}

IVideoSource *IVideoSource::create(VideoSourceType type) {
    IVideoSource *source = nullptr;
    switch (type) {
        case kVideoSourceFile:
            source = new VideoSourceFile;
            break;
        default:
            break;
    }
    return source;
}
