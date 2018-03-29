//
// Created by chao on 2018/2/6.
//

#ifndef RETROMEDIA_VIDEOSOURCE_H
#define RETROMEDIA_VIDEOSOURCE_H

#include <string>
#include "BaseType.h"
#include <list>
#include <memory>
#include <thread>

class IVideoSource {
public:
    virtual int open() = 0;
    virtual int start() = 0;
    virtual int close() = 0;
    virtual int setCallback(IDeliver *deliver) = 0;
    virtual int removeCallback(IDeliver *deliver) = 0;
    virtual int setSource(const std::string &str) { return ERRNUM();}
    virtual int setSource(int idx) {return ERRNUM();}
    virtual int setSourceInfo(int w, int h, VideoPictureFormat type) { return ERRNUM();}

    virtual ~IVideoSource(){}
    static IVideoSource *create(int type);
};

class VideoSource:public IVideoSource {
private:
    std::list<IDeliver *> mDelivers;
    std::shared_ptr<std::thread> mThread;
    bool mRun;
    void readThread();
    void waitThread();

public:
    VideoSource();
    virtual int start();
    virtual int stop();
    virtual int setCallback(IDeliver *deliver);
    virtual int removeCallback(IDeliver *deliver);
    virtual ~VideoSource();
    virtual bool readDate();

protected:
    virtual bool deliver(Buffer *buffer);
};

#endif //RETROMEDIA_VIDEOSOURCE_H
