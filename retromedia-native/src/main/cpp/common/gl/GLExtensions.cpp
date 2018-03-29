#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "GLExtensions.h"
#include <algorithm>

namespace openGl {
// ---------------------------------------------------------------------------

ANDROID_SINGLETON_STATIC_INSTANCE( GLExtensions )

GLExtensions::GLExtensions()
    : mHaveFramebufferObject(false)
{
}

void GLExtensions::initWithGLStrings(
        GLubyte const* vendor,
        GLubyte const* renderer,
        GLubyte const* version,
        GLubyte const* extensions)
{
    mVendor     = (char const*)vendor;
    mRenderer   = (char const*)renderer;
    mVersion    = (char const*)version;
    mExtensions = (char const*)extensions;

    char const* curr = (char const*)extensions;
    char const* head = curr;
    do {
        head = strchr(curr, ' ');
        std::string s(curr, head ? head-curr : strlen(curr));
        if (s.size()) {
            mExtensionList.push_back(s);
        }
        curr = head+1;
    } while (head);

    if (hasExtension("GL_OES_framebuffer_object")) {
        mHaveFramebufferObject = true;
    }
}

bool GLExtensions::hasExtension(char const* extension) const
{
    const std::string s(extension);
    std::vector<std::string>::const_iterator begin = mExtensionList.begin();
    std::vector<std::string>::const_iterator end = mExtensionList.end();

    std::vector<std::string>::const_iterator result = 
        std::find(begin, end, s);

    return result != end;
}

char const* GLExtensions::getVendor() const {
    return mVendor.c_str();
}

char const* GLExtensions::getRenderer() const {
    return mRenderer.c_str();
}

char const* GLExtensions::getVersion() const {
    return mVersion.c_str();
}

char const* GLExtensions::getExtension() const {
    return mExtensions.c_str();
}

// ---------------------------------------------------------------------------
}; // namespace android
