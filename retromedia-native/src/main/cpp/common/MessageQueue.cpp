//
// Created by chao on 2018/3/26.
//

#define LOG_TAG "MessageQueue"

#include "MessageQueue.h"
#include "DebugHelper.h"
#include "log_defs.h"
#include <thread>

struct MsgBox {
    MessageQueue::Msg msg;
};

typedef std::list<MsgBox> QueueList;

#define MQUEUE() ((QueueList *)(mQueue))

MessageQueue::Msg MessageQueue::poll() {
    return pollTime(INT64_MAX);
}

MessageQueue::Msg MessageQueue::pollTime(int64_t ms) {
    MsgBox box;
    box.msg.hwd = nullptr;
    std::unique_lock<std::mutex> lock_guard(mLock);
    SCOPEDDEBUG();
    QueueList::iterator it = MQUEUE()->begin();
    if (it == MQUEUE()->end()) {
        mCond.wait_for(lock_guard, std::chrono::duration<int64_t, std::milli>(ms));
        it = MQUEUE()->begin();
        if (it != MQUEUE()->end()) {
            box = *it;
            MQUEUE()->pop_front();
        }
    } else {
        box = *it;
        MQUEUE()->pop_front();
    }
    return box.msg;
}

MessageQueue::Msg MessageQueue::peek() {
    return MessageQueue::Msg();
}

MessageQueue::MessageQueue(int size, bool haveThread) {
    SCOPEDDEBUG();
    mSize = size;
    mExit = false;
    mFlush = true;
    mThread = nullptr;

    mQueue = new QueueList;
    MQUEUE()->clear();

    if (haveThread) {
        mThread = new std::thread(&MessageQueue::loop, this);
    }
}

int MessageQueue::size() {
    std::unique_lock<std::mutex> lock_guard(mLock);
    return MQUEUE()->size();
}

bool MessageQueue::post(MessageQueue::Msg &msg) {
    SCOPEDDEBUG();
    bool postDone = false;
    std::unique_lock<std::mutex> lock_guard(mLock);
    if (MQUEUE()->size() < mSize && !mExit) {
        MsgBox box;
        box.msg = msg;
        MQUEUE()->push_back(box);
        mCond.notify_one();
    }
    return postDone;
}

bool MessageQueue::postAndWait(MessageQueue::Msg &msg) {
    return postAndWait(msg, ULONG_MAX);
}

bool MessageQueue::postAndWait(MessageQueue::Msg &msg, long ms) {
    bool ret = true;

    if (post(msg)) {

    }

    return ret;
}

void MessageQueue::requestExit(bool sync) {
    SCOPEDDEBUG();
    mExit = true;
    if (sync && mThread) {
        {
            std::unique_lock<std::mutex> lock_guard(mLock);
            mCond.notify_all();
        }
        mThread->join();
    }

    if (mThread != nullptr)  {
        delete mThread;
        mThread = nullptr;
    }
}

MessageQueue::~MessageQueue() {
    SCOPEDDEBUG();
    requestExit(true);

    if (mThread) {
        delete mThread;
    }

    delete MQUEUE();
}

void MessageQueue::loop() {

    while (!mExit) {
        MsgBox box;
        box.msg = pollTime(500);
        if (box.msg.hwd) {
            box.msg.hwd->process(&box.msg);
        }
    }

    int remainMsg = size();
    if (mExit && remainMsg > 0 && mFlush) {
        ALOGI("Flush MessageQueue remain size %d", remainMsg);
        while (size() > 0) {
            MsgBox box;
            box.msg = pollTime(0);
            if (box.msg.hwd) {
                box.msg.hwd->process(&box.msg);
            }
        }
    }

}

