//
// Created by chao on 2018/2/6.
//

#ifndef RETROMEDIA_VIDEOBUFFERWRAPPER_H
#define RETROMEDIA_VIDEOBUFFERWRAPPER_H


#include "VideoType.h"

class VideoBufferWrapper {
public:
    static VideoBuffer *create();
    static int allocSize();
};


#endif //RETROMEDIA_VIDEOBUFFERWRAPPER_H
