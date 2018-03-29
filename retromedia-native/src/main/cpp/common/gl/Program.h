#ifndef SF_RENDER_ENGINE_PROGRAM_H
#define SF_RENDER_ENGINE_PROGRAM_H

#include <stdint.h>

#include <GLES2/gl2.h>

#include "Description.h"
#include "ProgramCache.h"
#include <string>

namespace openGl {

/*
 * Abstracts a GLSL program comprising a vertex and fragment shader
 */
class Program {
public:
    // known locations for position and texture coordinates
    enum { position=0, texCoords=1 };

    Program(const ProgramCache::Key& needs, const char* vertex, const char* fragment);
    ~Program();

    /* whether this object is usable */
    bool isValid() const;

    /* Binds this program to the GLES context */
    void use();

    /* Returns the location of the specified attribute */
    GLuint getAttrib(const char* name) const;

    /* Returns the location of the specified uniform */
    GLint getUniform(const char* name) const;

    /* set-up uniforms from the description */
    void setUniforms(const Description& desc);


private:
    GLuint buildShader(const char* source, GLenum type);
    std::string& dumpShader(std::string& result, GLenum type);

    // whether the initialization succeeded
    bool mInitialized;

    // Name of the OpenGL program and shaders
    GLuint mProgram;
    GLuint mVertexShader;
    GLuint mFragmentShader;

    /* location of the projection matrix uniform */
    GLint mProjectionMatrixLoc;

    /* location of the color matrix uniform */
    GLint mColorMatrixLoc;

    /* location of the texture matrix uniform */
    GLint mTextureMatrixLoc;

    /* location of the sampler uniform */
    GLint mSamplerLoc;

    /* location of the alpha plane uniform */
    GLint mAlphaPlaneLoc;

    /* location of the color uniform */
    GLint mColorLoc;
};


} /* namespace android */

#endif /* SF_RENDER_ENGINE_PROGRAM_H */
