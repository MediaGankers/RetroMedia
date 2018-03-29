#ifndef SF_GLES10RENDERENGINE_H_
#define SF_GLES10RENDERENGINE_H_

#include <stdint.h>
#include <sys/types.h>

#include "GLES11RenderEngine.h"

// ---------------------------------------------------------------------------
namespace openGl {
// ---------------------------------------------------------------------------

class GLES10RenderEngine : public GLES11RenderEngine {
    virtual ~GLES10RenderEngine();
protected:
    virtual void setupLayerBlending(bool premultipliedAlpha, bool opaque, int alpha);
};

// ---------------------------------------------------------------------------
}; // namespace openGl
// ---------------------------------------------------------------------------

#endif /* SF_GLES10RENDERENGINE_H_ */
