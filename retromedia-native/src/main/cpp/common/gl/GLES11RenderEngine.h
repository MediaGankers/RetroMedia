#ifndef SF_GLES11RENDERENGINE_H_
#define SF_GLES11RENDERENGINE_H_

#include <stdint.h>
#include <sys/types.h>

#include <GLES/gl.h>
#include <gl/Transform.h>

#include <gl/RenderEngine.h>
#include <gl/util.h>

// ---------------------------------------------------------------------------
namespace openGl {
// ---------------------------------------------------------------------------

class GLES11RenderEngine : public RenderEngine {
    GLuint mProtectedTexName;
    GLint mMaxViewportDims[2];
    GLint mMaxTextureSize;

    virtual void bindImageAsFramebuffer(EGLImageKHR image,
            uint32_t* texName, uint32_t* fbName, uint32_t* status);
    virtual void unbindFramebuffer(uint32_t texName, uint32_t fbName);

public:
    GLES11RenderEngine();

protected:
    virtual ~GLES11RenderEngine();

    virtual void dump(std::string& result);
    virtual void setViewportAndProjection(size_t vpw, size_t vph,
            Rect sourceCrop, size_t hwh, bool yswap, Transform::orientation_flags rotation);
    virtual void setupLayerBlending(bool premultipliedAlpha, bool opaque, int alpha);
    virtual void setupDimLayerBlending(int alpha);
    virtual void setupLayerTexturing(const Texture& texture);
    virtual void setupLayerBlackedOut();
    virtual void setupFillWithColor(float r, float g, float b, float a) ;
    virtual void disableTexturing();
    virtual void disableBlending();

    virtual void drawMesh(const Mesh& mesh);

    virtual size_t getMaxTextureSize() const;
    virtual size_t getMaxViewportDims() const;
};

// ---------------------------------------------------------------------------
}; // namespace android
// ---------------------------------------------------------------------------

#endif /* SF_GLES11RENDERENGINE_H_ */
