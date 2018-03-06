//
// Created by chao on 2018/2/6.
//
#define LOG_TAG "VideoSourceFile.cpp"

#include "VideoSourceFile.h"
#include "log_defs.h"
#include "VideoBufferWrapper.h"
#include "libyuv.h"

#define FRAME_RATE 30

int VideoSourceFile::open() {
    int ret = 0;
    if (mFile == nullptr) {
        mFile = fopen(mName.c_str(), "r");
        if (mFile == nullptr) {
            ret = ERRNUM();
        }
    }
    return ret;
}

int VideoSourceFile::start() {
    return VideoSource::start();
}

int VideoSourceFile::close() {
    if (mFile != nullptr) {
        fclose(mFile);
        mFile = nullptr;
    }
    return 0;
}

int VideoSourceFile::setSource(const std::string &str) {
    int ret = 0;
    if (str.empty() || (ret = access(str.c_str(), R_OK)) < 0) {
        ALOGE("setSource path %s failed (%s)", str.c_str(), strerror(ret));
        return ERRNUM();
    }

    mName = str;
    ALOGD("setSource path %s Succeed.", mName.c_str());
    return 0;
}

VideoSourceFile::~VideoSourceFile() {
    close();
    if (mVideoBuffer) {
        mVideoBuffer->release();
    }
}

VideoSourceFile::VideoSourceFile():
mWidth(0),
mHeight(0),
mFormat(kVideoPictureFormatUnknown),
mFile(nullptr),
mVideoBuffer(nullptr){

}

int VideoSourceFile::setSourceInfo(int w, int h, VideoPictureFormat type) {
    mWidth = w;
    mHeight = h;
    mFormat = type;
    ALOGD("setSourceInfo w %d h %d format %d", w, h, type);
    return 0;
}

bool VideoSourceFile::readDate() {
    if (!mFile) {
        return false;
    }

    if (mFormat != kVideoPictureFormatI420) {
        return false;
    }

    if (mWidth <=0 || mHeight <=0 ) {
        return false;
    }

    int size = mWidth * mHeight * 3 / 2;

    if (mVideoBuffer == nullptr) {
        mVideoBuffer = VideoBufferWrapper::create();
        mVideoBuffer->alloc(size);
    }

    usleep(1000 / FRAME_RATE);
    int rsize = fread(mVideoBuffer->date(), 1, size, mFile);

    if (rsize <=0 ) {
        rewind(mFile);
        rsize = fread(mVideoBuffer->date(), 1, size, mFile);
    }

    bool deliverOk = VideoSource::deliver(mVideoBuffer);

    if (!deliverOk) {
        ALOGW("No deliver.....");
    }

    return rsize > 0;
}
