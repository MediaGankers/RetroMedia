#include <stdint.h>
#include <string.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Description.h"

namespace openGl {

Description::Description() :
    mUniformsDirty(true) {
    mPlaneAlpha = 1.0f;
    mPremultipliedAlpha = false;
    mOpaque = true;
    mTextureEnabled = false;
    mColorMatrixEnabled = false;

    memset(mColor, 0, sizeof(mColor));
}

Description::~Description() {
}

void Description::setPlaneAlpha(GLclampf planeAlpha) {
    if (planeAlpha != mPlaneAlpha) {
        mUniformsDirty = true;
        mPlaneAlpha = planeAlpha;
    }
}

void Description::setPremultipliedAlpha(bool premultipliedAlpha) {
    if (premultipliedAlpha != mPremultipliedAlpha) {
        mPremultipliedAlpha = premultipliedAlpha;
    }
}

void Description::setOpaque(bool opaque) {
    if (opaque != mOpaque) {
        mOpaque = opaque;
    }
}

void Description::setTexture(const Texture& texture) {
    mTexture = texture;
    mTextureEnabled = true;
    mUniformsDirty = true;
}

void Description::disableTexture() {
    mTextureEnabled = false;
}

void Description::setColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha) {
    mColor[0] = red;
    mColor[1] = green;
    mColor[2] = blue;
    mColor[3] = alpha;
    mUniformsDirty = true;
}

void Description::setProjectionMatrix(const mat4& mtx) {
    mProjectionMatrix = mtx;
    mUniformsDirty = true;
}

void Description::setColorMatrix(const mat4& mtx) {
    const mat4 identity;
    mColorMatrix = mtx;
    mColorMatrixEnabled = (mtx != identity);
}

const mat4& Description::getColorMatrix() const {
    return mColorMatrix;
}


} /* namespace android */
