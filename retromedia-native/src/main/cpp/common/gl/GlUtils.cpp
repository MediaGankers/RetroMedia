//
// Created by chao on 2018/3/22.
//

#include <assert.h>
#include "GlUtils.h"
#include "RenderEngine.h"

inline void setTextureParam(GLint type, GLenum filter, GLenum wrap)
{
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, filter);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, wrap);
}

void GlUtils::deleteTexture(int i, GLuint *tex) {
    glDeleteTextures(i, tex);
}

GLuint GlUtils::createTexture2d(int w, int h, void *data) {
    assert(w != 0 && h != 0);
    GLuint texID = createTexture();

    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    setTextureParam(GL_TEXTURE_2D, GL_LINEAR, GL_CLAMP_TO_EDGE);
    return texID;
}

EGLSurface GlUtils::createSurface(int w, int h) {
    EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    EGLint pBufferAttrib[] = {
            EGL_WIDTH, w,
            EGL_HEIGHT, h,
            EGL_LARGEST_PBUFFER, EGL_TRUE,
            EGL_NONE
    };
    return eglCreatePbufferSurface(dpy, openGl::RenderEngine::chooseEglConfig(dpy, 1), pBufferAttrib);
}

void GlUtils::destroyEglSurface(EGLSurface &surface) {
    EGLDisplay dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    if (surface != EGL_NO_SURFACE) {
        eglDestroySurface(dpy, surface);
    }
}

bool GlUtils::createFrameBuffer(int count, GLuint *value) {
    glGenFramebuffers(count, value);
    return count;
}

GLuint GlUtils::createTexture() {
    GLuint tex = 0;
    glGenTextures(1, &tex);
    return tex;
}

void GlUtils::deleteFrameBuffer(int i, uint32_t *pInt) {
    glDeleteFramebuffers(1, pInt);
}

