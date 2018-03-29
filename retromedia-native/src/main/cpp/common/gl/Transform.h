#ifndef ANDROID_TRANSFORM_H
#define ANDROID_TRANSFORM_H

#include <stdint.h>
#include <sys/types.h>
#include "util.h"
#include "vec3.h"
#include "vec2.h"
#include "mat4.h"
#include "Region.h"


namespace openGl {


// ---------------------------------------------------------------------------

class Transform
{
public:
                    Transform();
                    Transform(const Transform&  other);
           explicit Transform(uint32_t orientation);
                    ~Transform();

            enum orientation_flags {
                ROT_0   = 0x00000000,
                FLIP_H  = HAL_TRANSFORM_FLIP_H,
                FLIP_V  = HAL_TRANSFORM_FLIP_V,
                ROT_90  = HAL_TRANSFORM_ROT_90,
                ROT_180 = FLIP_H|FLIP_V,
                ROT_270 = ROT_180|ROT_90,
                ROT_INVALID = 0x80
            };

            enum type_mask {
                IDENTITY            = 0,
                TRANSLATE           = 0x1,
                ROTATE              = 0x2,
                SCALE               = 0x4,
                UNKNOWN             = 0x8
            };

            // query the transform
            bool        transformed() const;
            bool        preserveRects() const;
            uint32_t    getType() const;
            uint32_t    getOrientation() const;

            const vec3& operator [] (size_t i) const;  // returns column i
            float   tx() const;
            float   ty() const;

            // modify the transform
            void        reset();
            void        set(float tx, float ty);
            void        set(float a, float b, float c, float d);
            status_t    set(uint32_t flags, float w, float h);

            // transform data
            Rect    makeBounds(int w, int h) const;
            vec2    transform(int x, int y) const;
            Region  transform(const Region& reg) const;
            Rect    transform(const Rect& bounds) const;
            Transform operator * (const Transform& rhs) const;

            Transform inverse() const;

            // for debugging
            void dump(const char* name) const;

private:
    struct mat33 {
        vec3 v[3];
        inline const vec3& operator [] (int i) const { return v[i]; }
        inline vec3& operator [] (int i) { return v[i]; }
    };

    enum { UNKNOWN_TYPE = 0x80000000 };

    // assumes the last row is < 0 , 0 , 1 >
    vec2 transform(const vec2& v) const;
    vec3 transform(const vec3& v) const;
    uint32_t type() const;
    static bool absIsOne(float f);
    static bool isZero(float f);

    mat33               mMatrix;
    mutable uint32_t    mType;
};

// ---------------------------------------------------------------------------
}; // namespace android

#endif /* ANDROID_TRANSFORM_H */
