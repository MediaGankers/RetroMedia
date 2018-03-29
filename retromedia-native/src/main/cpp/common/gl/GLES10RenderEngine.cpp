#include <GLES/gl.h>

#include "GLES10RenderEngine.h"

// ---------------------------------------------------------------------------
namespace openGl{
// ---------------------------------------------------------------------------

GLES10RenderEngine::~GLES10RenderEngine() {
}

void GLES10RenderEngine::setupLayerBlending(
    bool premultipliedAlpha, bool opaque, int alpha) {
    // OpenGL ES 1.0 doesn't support texture combiners.
    // This path doesn't properly handle opaque layers that have non-opaque
    // alpha values. The alpha channel will be copied into the framebuffer or
    // screenshot, so if the framebuffer or screenshot is blended on top of
    // something else,  whatever is below the window will incorrectly show
    // through.
    if (CC_UNLIKELY(alpha < 0xFF)) {
        GLfloat floatAlpha = alpha * (1.0f / 255.0f);
        if (premultipliedAlpha) {
            glColor4f(floatAlpha, floatAlpha, floatAlpha, floatAlpha);
        } else {
            glColor4f(1.0f, 1.0f, 1.0f, floatAlpha);
        }
        glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    } else {
        glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    }

    if (alpha < 0xFF || !opaque) {
        glEnable(GL_BLEND);
        glBlendFunc(premultipliedAlpha ? GL_ONE : GL_SRC_ALPHA,
                    GL_ONE_MINUS_SRC_ALPHA);
    } else {
        glDisable(GL_BLEND);
    }
}

// ---------------------------------------------------------------------------
}; // namespace openGl
// ---------------------------------------------------------------------------
