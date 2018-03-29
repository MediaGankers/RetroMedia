//
// Created by chao on 2018/2/6.
//

#ifndef RETROMEDIA_VIDEOSOURCEFILE_H
#define RETROMEDIA_VIDEOSOURCEFILE_H

#include "VideoSource.h"
#include "common/VideoBufferPool.h"
class VideoSourceFile: public VideoSource {
private:
    std::string mName;
    int mWidth;
    int mHeight;
    VideoPictureFormat mFormat;
    FILE *mFile;
    VideoBufferPool *mVideoBufferPool;
protected:
    virtual bool readDate();
public:
    VideoSourceFile();
    virtual int open();
    virtual int start();
    virtual int close();
    virtual int setSource(const std::string &str);
    virtual int setSourceInfo(int w, int h, VideoPictureFormat type);
    virtual ~VideoSourceFile();
};
#endif //RETROMEDIA_VIDEOSOURCEFILE_H
