#ifndef ANDROID_SF_GLEXTENSION_H
#define ANDROID_SF_GLEXTENSION_H

#include <stdint.h>
#include <sys/types.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <string>
#include <vector>
#include <gl/util.h>

namespace openGl{
// ---------------------------------------------------------------------------

class GLExtensions : public Singleton<GLExtensions>
{
    friend class Singleton<GLExtensions>;

    bool mHaveFramebufferObject : 1;

    std::string mVendor;
    std::string mRenderer;
    std::string mVersion;
    std::string mExtensions;
    std::vector<std::string> mExtensionList;

    GLExtensions(const GLExtensions&);
    GLExtensions& operator = (const GLExtensions&);

protected:
    GLExtensions();

public:

    inline bool haveFramebufferObject() const {
        return mHaveFramebufferObject;
    }

    void initWithGLStrings(
            GLubyte const* vendor,
            GLubyte const* renderer,
            GLubyte const* version,
            GLubyte const* extensions);

    char const* getVendor() const;
    char const* getRenderer() const;
    char const* getVersion() const;
    char const* getExtension() const;

    bool hasExtension(char const* extension) const;
};


// ---------------------------------------------------------------------------
}; // namespace android

#endif // ANDROID_SF_GLEXTENSION_H
