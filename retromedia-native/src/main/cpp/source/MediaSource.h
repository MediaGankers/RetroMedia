//
// Created by chao on 2018/3/25.
//

#ifndef RETROMEDIA_ISOURCE_H
#define RETROMEDIA_ISOURCE_H

#include <list>
#include <mutex>
#include <condition_variable>
#include "MetaData.h"
#include "common/Buffer.h"
#include "common/BaseType.h"

class MediaSource {
public:
    struct Message {
        int what;
        int arg0;
        int arg1;
        void *obj;
        Message() {
            memset(this, 0, sizeof(this));
        }
    };

    bool getNotify(Message &message);

public:
    enum Status {
        kIdle,
        kInit,
        kStarted,
        kStoped,
        kPreparing,
        kPreared,
        kPause,
        kError,
        kEnd
    };

protected:
    bool deliver(Buffer *buffer, StreamType type);

    inline const char * statusToString(Status stat) {
        switch (stat) {
            case kIdle:
                return "Idle";
            case kInit:
                return "Init";
            case kStarted:
                return "Started";
            case kStoped:
                return "Stoped";
            case kPreared:
                return "Prepared";
            case kPreparing:
                return "Preparing";
            case kEnd:
                return "End";
            case kError:
                return "Error";
            case kPause:
                return "Pause";
        }
        return "Unknown";
    }

public:
    MediaSource();

    virtual int start();

    virtual int stop();

    virtual int pause();

    virtual int prepare();

    virtual int release();

    virtual int reset();

    virtual void addDeliver(IDeliver *);

    virtual void removeDeliver(IDeliver *);

    virtual int setParameter(int key, void *obj);

    Status status();

    Status setStatus(Status status);

    virtual void needMsgQueue() {

    }

    virtual ~MediaSource();

protected:
    std::list<IDeliver *> mDelivers;

    Status mStatus;
    std::mutex mStatLock;
    std::condition_variable mStatLockCond;

protected:
    MetaData *mMetaData;
};


#endif //RETROMEDIA_ISOURCE_H
