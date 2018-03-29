#include <string.h>

#include <gl/Texture.h>

namespace openGl {

Texture::Texture() :
    mTextureName(0), mTextureTarget(TEXTURE_2D),
    mWidth(0), mHeight(0), mFiltering(false) {
}

Texture::Texture(Target textureTarget, uint32_t textureName) :
            mTextureName(textureName), mTextureTarget(textureTarget),
            mWidth(0), mHeight(0), mFiltering(false) {
}

void Texture::init(Target textureTarget, uint32_t textureName) {
    mTextureName = textureName;
    mTextureTarget = textureTarget;
}

Texture::~Texture() {
}


void Texture::setMatrix(float const* matrix) {
    mTextureMatrix = mat4(matrix);
}

void Texture::setFiltering(bool enabled) {
    mFiltering = enabled;
}

void Texture::setDimensions(size_t width, size_t height) {
    mWidth = width;
    mHeight = height;
}

uint32_t Texture::getTextureName() const {
    return mTextureName;
}

uint32_t Texture::getTextureTarget() const {
    return mTextureTarget;
}

const mat4& Texture::getMatrix() const {
    return mTextureMatrix;
}

bool Texture::getFiltering() const {
    return mFiltering;
}

size_t Texture::getWidth() const {
    return mWidth;
}

size_t Texture::getHeight() const {
    return mHeight;
}

} /* namespace android */
