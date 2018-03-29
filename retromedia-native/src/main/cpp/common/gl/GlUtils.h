//
// Created by chao on 2018/3/22.
//

#ifndef RETROMEDIA_GLUTILS_H
#define RETROMEDIA_GLUTILS_H
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>

class GlUtils {
public:

    static GLuint createTexture2d(int w, int h, void *data);

    static GLuint createTexture();

    static void deleteTexture(int i, GLuint *tex);

    static EGLSurface createSurface(int w, int h);

    static void destroyEglSurface(EGLSurface &surface);

    static bool createFrameBuffer(int count, GLuint *value);

    static void deleteFrameBuffer(int i, uint32_t *pInt);
};


#endif //RETROMEDIA_GLUTILS_H
