//
// Created by chao on 2018/3/11.
//

#include "Reference.h"

Reference::Reference(int atomic) {
    mAtomic = atomic;
}

int Reference::addRef() {
    return mAtomic++;
}

int Reference::decRef() {
    return mAtomic--;
}

int Reference::count() {
    return mAtomic;
}

Reference::~Reference() {

}
