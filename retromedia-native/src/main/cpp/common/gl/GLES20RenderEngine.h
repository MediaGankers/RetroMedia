#ifndef SF_GLES20RENDERENGINE_H_
#define SF_GLES20RENDERENGINE_H_

#include <stdint.h>
#include <sys/types.h>

#include <GLES2/gl2.h>
#include <gl/Transform.h>

#include <gl/RenderEngine.h>
#include "ProgramCache.h"
#include "Description.h"
#include <vector>

// ---------------------------------------------------------------------------
namespace openGl {
// ---------------------------------------------------------------------------

class String8;
class Mesh;
class Texture;

class GLES20RenderEngine : public RenderEngine {
    GLuint mProtectedTexName;
    GLint mMaxViewportDims[2];
    GLint mMaxTextureSize;
    GLuint mVpWidth;
    GLuint mVpHeight;

    struct Group {
        GLuint texture;
        GLuint fbo;
        GLuint width;
        GLuint height;
        mat4 colorTransform;
    };

    Description mState;
    std::vector<Group> mGroupStack;

    virtual void bindImageAsFramebuffer(EGLImageKHR image,
            uint32_t* texName, uint32_t* fbName, uint32_t* status);
    virtual void unbindFramebuffer(uint32_t texName, uint32_t fbName);

public:
    GLES20RenderEngine();

protected:
    virtual ~GLES20RenderEngine();

    virtual void dump(std::string& result);
    virtual void setViewportAndProjection(size_t vpw, size_t vph,
            Rect sourceCrop, size_t hwh, bool yswap, Transform::orientation_flags rotation);
    virtual void setupLayerBlending(bool premultipliedAlpha, bool opaque, int alpha);
    virtual void setupDimLayerBlending(int alpha);
    virtual void setupLayerTexturing(const Texture& texture);
    virtual void setupLayerBlackedOut();
    virtual void setupFillWithColor(float r, float g, float b, float a);
    virtual mat4 setupColorTransform(const mat4& colorTransform);
    virtual void disableTexturing();
    virtual void disableBlending();

    virtual void drawMesh(const Mesh& mesh);

    virtual size_t getMaxTextureSize() const;
    virtual size_t getMaxViewportDims() const;
};

// ---------------------------------------------------------------------------
}; // namespace android
// ---------------------------------------------------------------------------

#endif /* SF_GLES20RENDERENGINE_H_ */