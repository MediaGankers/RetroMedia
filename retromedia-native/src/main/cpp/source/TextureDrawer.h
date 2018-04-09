//
// Created by chao on 2018/4/4.
//

#ifndef RETROMEDIA_TEXTUREDRAWER_H
#define RETROMEDIA_TEXTUREDRAWER_H


#include <common/gl/RenderEngine.h>
#include <common/gl/Texture.h>
#include <common/gl/Mesh.h>
#include "TexBufferPool.h"

class TextureDrawer {
protected:
    openGl::RenderEngine *mEngine;
    openGl::Texture mTexture;
    EGLSurface mSurface;
    openGl::Mesh mMesh;
    int mWindowWidth;
    int mWindowHeight;


public:
    TextureDrawer();

    virtual int init();

    virtual int draw(const TexBuffer &buffer);

    virtual int makeCurrent(EGLNativeWindowType surface);

    virtual int setProjectionMatrix(const openGl::mat4 &mat4);

    virtual int setTextureMatrix(const openGl::mat4 &mat4);

    virtual int setColorMatrix(const openGl::mat4 &mat4);

    virtual ~TextureDrawer();

    void updateWindowSize(int w, int h);
};


#endif //RETROMEDIA_TEXTUREDRAWER_H
