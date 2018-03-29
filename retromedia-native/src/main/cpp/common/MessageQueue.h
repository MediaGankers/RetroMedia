//
// Created by chao on 2018/3/26.
//

#ifndef RETROMEDIA_MESSAGEQUEUE_H
#define RETROMEDIA_MESSAGEQUEUE_H

#include <stdint.h>
#include <semaphore.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>

class MessageQueue {
public:
    struct Msg;
    class MessageHandle {
    public:
        virtual void process(Msg *msg) = 0;
    };

public:
    struct Msg {
        int what;
        MessageHandle *hwd;
        void *data;
    };

private:
    void loop();
public:
    MessageQueue(int size = 100, bool haveThread = true);
    virtual Msg poll();
    virtual Msg pollTime(int64_t ms);
    virtual Msg peek();
    virtual int size();
    virtual bool post(Msg &msg);
    virtual bool postAndWait(Msg &msg);
    virtual bool postAndWait(Msg &msg, long ms);
    virtual void requestExit(bool sync = true);
    ~MessageQueue();

private:
    void *mQueue;
    std::thread *mThread;
    std::mutex mLock;
    std::condition_variable mCond;
    int mSize;
    bool mExit;
};


#endif //RETROMEDIA_MESSAGEQUEUE_H
