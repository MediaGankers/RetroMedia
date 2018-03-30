#ifndef SF_RENDERENGINE_H_
#define SF_RENDERENGINE_H_

#include <stdint.h>
#include <sys/types.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <string>
#include "util.h"
#include "Transform.h"
#include "mat4.h"

#define EGL_NO_CONFIG ((EGLConfig)0)

// ---------------------------------------------------------------------------
namespace openGl {
// ---------------------------------------------------------------------------

class Region;
class Mesh;
class Texture;

class RenderEngine {
    enum GlesVersion {
        GLES_VERSION_1_0    = 0x10000,
        GLES_VERSION_1_1    = 0x10001,
        GLES_VERSION_2_0    = 0x20000,
        GLES_VERSION_3_0    = 0x30000,
    };
    static GlesVersion parseGlesVersion(const char* str);

    EGLConfig mEGLConfig;
    EGLContext mEGLContext;
    EGLDisplay mDefaultdpy;

    void setDefaultDisplay(EGLDisplay pVoid);

    void setEGLHandles(EGLConfig config, EGLContext ctxt);

    virtual void bindImageAsFramebuffer(EGLImageKHR image, uint32_t* texName, uint32_t* fbName, uint32_t* status) = 0;
    virtual void unbindFramebuffer(uint32_t texName, uint32_t fbName) = 0;

protected:
    RenderEngine();
    virtual ~RenderEngine();

public:
    static RenderEngine* create(EGLContext ctx = nullptr);

    static EGLConfig chooseEglConfig(EGLDisplay display, int format);

    void makeCurrent(EGLSurface surface);

    EGLSurface createSurface(int w, int h);
    void destroySurface(EGLSurface surface);

    // dump the extension strings. always call the base class.
    virtual void dump(std::string& result);

    // helpers
    void flush();
    void clearWithColor(float red, float green, float blue, float alpha);
    void fillRegionWithColor(const Region& region, uint32_t height,
            float red, float green, float blue, float alpha);

    // common to all GL versions
    void setScissor(uint32_t left, uint32_t bottom, uint32_t right, uint32_t top);
    void disableScissor();
    void genTextures(size_t count, uint32_t* names);
    void deleteTextures(size_t count, uint32_t const* names);
    void readPixels(size_t l, size_t b, size_t w, size_t h, uint32_t* pixels);

    class BindImageAsFramebuffer {
        RenderEngine& mEngine;
        uint32_t mTexName, mFbName;
        uint32_t mStatus;
    public:
        BindImageAsFramebuffer(RenderEngine& engine, EGLImageKHR image);
        ~BindImageAsFramebuffer();
        int getStatus() const;
    };

    // set-up
    virtual void checkErrors() const;
    virtual void setViewportAndProjection(size_t vpw, size_t vph,
            Rect sourceCrop, size_t hwh, bool yswap, Transform::orientation_flags rotation) = 0;
    virtual void setupLayerBlending(bool premultipliedAlpha, bool opaque, int alpha) = 0;
    virtual void setupDimLayerBlending(int alpha) = 0;
    virtual void setupLayerTexturing(const Texture& texture) = 0;
    virtual void setupLayerBlackedOut() = 0;
    virtual void setupFillWithColor(float r, float g, float b, float a) = 0;

    virtual mat4 setupColorTransform(const mat4& /* colorTransform */) {
        return mat4();
    }

    virtual void disableTexturing() = 0;
    virtual void disableBlending() = 0;

    // drawing
    virtual void drawMesh(const Mesh& mesh) = 0;

    // queries
    virtual size_t getMaxTextureSize() const = 0;
    virtual size_t getMaxViewportDims() const = 0;

    EGLConfig getEGLConfig() const;
    EGLContext getEGLContext() const;

    void release();

};

// ---------------------------------------------------------------------------
}; // namespace CCStone
// ---------------------------------------------------------------------------

#endif /* SF_RENDERENGINE_H_ */
