#ifndef SF_RENDER_ENGINE_TEXTURE_H
#define SF_RENDER_ENGINE_TEXTURE_H

#include <stdint.h>
#include "mat4.h"

namespace openGl {

class Texture {
    uint32_t mTextureName;
    uint32_t mTextureTarget;
    size_t mWidth;
    size_t mHeight;
    bool mFiltering;
    mat4 mTextureMatrix;

public:
    enum Target { TEXTURE_2D = 0x0DE1, TEXTURE_EXTERNAL = 0x8D65 };

    Texture();
    Texture(Target textureTarget, uint32_t textureName);
    ~Texture();

    void init(Target textureTarget, uint32_t textureName);

    void setMatrix(float const* matrix);
    void setFiltering(bool enabled);
    void setDimensions(size_t width, size_t height);

    uint32_t getTextureName() const;
    uint32_t getTextureTarget() const;

    const mat4& getMatrix() const;
    bool getFiltering() const;
    size_t getWidth() const;
    size_t getHeight() const;
};

} /* namespace CCStone */
#endif /* SF_RENDER_ENGINE_TEXTURE_H */
