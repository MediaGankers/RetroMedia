//
// Created by chao on 2018/3/11.
//

#ifndef LIVETEST_REFERENCE_H
#define LIVETEST_REFERENCE_H

#include <atomic>

class Reference {
private:
    std::atomic<int> mAtomic;
public:
    Reference(int atomic = 0);
    virtual int addRef();
    virtual int decRef();
    virtual int count();
    virtual ~Reference();
};


#endif //LIVETEST_REFERENCE_H
