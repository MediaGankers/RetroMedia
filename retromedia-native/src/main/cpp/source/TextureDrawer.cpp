//
// Created by chao on 2018/4/4.
//
#define LOG_TAG "TextureDrawer"

#include <common/BaseType.h>
#include <common/DebugHelper.h>
#include "TextureDrawer.h"

int TextureDrawer::draw(const TexBuffer &buffer) {

    SCOPEDDEBUG();
    int ret = ERRNUM();
    float const *array = nullptr;
    int stride = 0;

    if (mEngine == nullptr) {
        ret = ERRNUM();
        goto exit;
    }

    mTexture.init(buffer.texType() == GL_TEXTURE_EXTERNAL_OES ? openGl::Texture::TEXTURE_EXTERNAL : openGl::Texture::TEXTURE_2D
            , buffer.texId());

    mTexture.setDimensions(buffer.width(), buffer.height());

    mEngine->setupLayerTexturing(mTexture);

    ALOGI("setupLayerTexture %d w %d h %d ", mTexture.getTextureName(), mTexture.getWidth(), mTexture.getHeight());

    array =  mMesh.getPositions();
    stride = mMesh.getStride();

    glViewport(0, 0, 1920, 1080);
    mEngine->drawMesh(mMesh);

    //mEngine->clearWithColor(1.0f, 0.f,0.f,1.0f);
    mEngine->checkErrors();

    mEngine->swapBuffer(mSurface);
exit:
    return ret;
}

int TextureDrawer::makeCurrent(EGLNativeWindowType surface) {
    if (mEngine != nullptr) {
        SCOPEDDEBUG();
        EGLSurface s = nullptr;
        if (surface) {
           s = mEngine->createSurface(surface);
        }else {
            s = mEngine->createSurface(1, 1);
        }

        if (mSurface) {
            mEngine->destroySurface(mSurface);
        }

        mSurface = s;
        mEngine->makeCurrent(mSurface);
    }
    return 0;
}

int TextureDrawer::setProjectionMatrix(const openGl::mat4 &mat4) {
    return ERRNUM();
}

int TextureDrawer::setTextureMatrix(const openGl::mat4 &mat4) {
    mTexture.setMatrix(mat4.asArray());
    return 0;
}

int TextureDrawer::setColorMatrix(const openGl::mat4 &mat4) {
    SCOPEDDEBUG();
    if (mEngine) {
        mEngine->setupColorTransform(mat4);
        return 0;
    }
    return ERRNUM();
}

int TextureDrawer::init() {
    SCOPEDDEBUG();
    mEngine = openGl::RenderEngine::createWithContext();

    if (mEngine) {
        mSurface = mEngine->createSurface(1, 1);
        mEngine->makeCurrent(mSurface);
    }

    openGl::Mesh::VertexArray<openGl::vec2> position(mMesh.getPositionArray<openGl::vec2>());
    openGl::Transform tr;
    position[0] = tr.transform(-1, -1);
    position[1] = tr.transform(1, -1);
    position[2] = tr.transform(1, 1);
    position[3] = tr.transform(-1, 1);

    tr.reset();
    openGl::Mesh::VertexArray<openGl::vec2> coord(mMesh.getTexCoordArray<openGl::vec2>());

    coord[0] = tr.transform(0, 1);
    coord[1] = tr.transform(0, 0);
    coord[2] = tr.transform(1, 0);
    coord[3] = tr.transform(1, 1);
    return mEngine ? 0: ERRNUM();
}

TextureDrawer::TextureDrawer(): mMesh(openGl::Mesh::TRIANGLE_FAN, 4,2,2){
    mEngine = nullptr;
    mSurface = nullptr;
    mWindowHeight = mWindowWidth = 0;
}

TextureDrawer::~TextureDrawer() {
    SCOPEDDEBUG();
    if (mSurface) {
        mEngine->destroySurface(mSurface);
        mSurface = nullptr;
    }

    if (mEngine) {
        mEngine->release();
        mEngine = nullptr;
    }
}

void TextureDrawer::updateWindowSize(int w, int h) {
    mWindowWidth = w;
    mWindowHeight = h;
}
