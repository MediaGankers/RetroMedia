//
// Created by chao on 2018/3/26.
//

#ifndef RETROMEDIA_CAMERAMEDIASOURCEONANDROID_H
#define RETROMEDIA_CAMERAMEDIASOURCEONANDROID_H


#include <JNIHelper.h>
#include <thread>
#include <MessageQueue.h>
#include <gl/Mesh.h>
#include <TexBufferPool.h>
#include "source/CameraMediaSource.h"
#include "RenderEngine.h"
#include "Texture.h"

class CameraMediaSourceOnAndroid: public CameraMediaSource, MessageQueue::MessageHandle {
public:
    enum {
        kKeyCamSourceFront = 'acmf', // bool (int32_t)
        kKeyCamSourceMirror = 'acmM' // bool (int32_t)
    };

    enum {
        kMsgCreateOESTExture = 1,
        kMsgCreateRenderEngine,
        kMsgOnFrameAvailable,
        kMsgReleaseSource
    };
private:
    JNIHelper::GlobalObject mCamJava;
private:
    void process(MessageQueue::Msg *msg);

public:
    CameraMediaSourceOnAndroid();

    virtual int start();

    virtual int stop();

    virtual int pause();

    virtual int prepare();

    virtual int release();

    virtual int reset();

    virtual int setParameter(int key, void *obj);

    ~CameraMediaSourceOnAndroid();

    int getOESTexture();

    int onFrameAvailable(int64_t timestamp, float matrix[]);

    openGl::RenderEngine *mRenderEngine;
    EGLSurface mEglSurface;
    uint32_t mFbo;
    TexBufferPool *mTexPool;
    MessageQueue mRenderQueue;
    std::mutex mSyncLock;
    std::condition_variable mSyncCond;
    openGl::Texture mCameraTex;

    void createRenderEngine();

    void draw();

    openGl::Mesh *mMesh;

    int stopInternal();
};


#endif //RETROMEDIA_CAMERAMEDIASOURCEONANDROID_H
