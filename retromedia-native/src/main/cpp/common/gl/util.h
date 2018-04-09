#ifndef CCSTONE_OPENGL_UTIL_H_
#define CCSTONE_OPENGL_UTIL_H_
#include <stdio.h>
#include <libavutil/time.h>
#include "log_defs.h"
#include <mutex>
#include <unistd.h>

namespace openGl {

#define NO_ERROR 0
#define NO_MEMORY -22
#define BAD_VALUE -23
#define NAME_NOT_FOUND -24

#ifndef CC_UNLIKELY
#define CC_UNLIKELY(exper) exper
#endif

#ifndef CC_LIKELY
#define CC_LIKELY(exper) exper
#endif

#ifndef EGL_FRAMEBUFFER_TARGET_ANDROID
#define EGL_FRAMEBUFFER_TARGET_ANDROID 0
#endif

inline int __gl_assert()
{
  __builtin_trap();  
  return 0;
}

#ifdef __cplusplus
    template<bool> struct CompileTimeAssert;
    template<> struct CompileTimeAssert<true> {}; 
#define COMPILE_TIME_ASSERT(_exp) \
    template class CompileTimeAssert< (_exp) >;
#endif
#define COMPILE_TIME_ASSERT_FUNCTION_SCOPE(_exp) \
    CompileTimeAssert<( _exp )>();

    // ---------------------------------------------------------------------------

#ifdef __cplusplus
    template<bool C, typename LSH, typename RHS> struct CompileTimeIfElse;
    template<typename LHS, typename RHS> 
        struct CompileTimeIfElse<true,  LHS, RHS> { typedef LHS TYPE; };
    template<typename LHS, typename RHS> 
        struct CompileTimeIfElse<false, LHS, RHS> { typedef RHS TYPE; };
#endif

    enum {
        /* flip source image horizontally (around the vertical axis) */
        HAL_TRANSFORM_FLIP_H    = 0x01,
        /* flip source image vertically (around the horizontal axis)*/
        HAL_TRANSFORM_FLIP_V    = 0x02,
        /* rotate source image 90 degrees clockwise */
        HAL_TRANSFORM_ROT_90    = 0x04,
        /* rotate source image 180 degrees */
        HAL_TRANSFORM_ROT_180   = 0x03,
        /* rotate source image 270 degrees clockwise */
        HAL_TRANSFORM_ROT_270   = 0x07,
        /* don't use. see system/window.h */
        HAL_TRANSFORM_RESERVED  = 0x08,
    };

typedef long long nsecs_t;
typedef int32_t status_t;

inline nsecs_t systemTime() 
{
    struct timeval time;
    gettimeofday(&time,NULL);

    nsecs_t interval = 0;
    interval = 1000000*(time.tv_sec - time.tv_sec) + time.tv_usec;
    return interval * 1000;
}

template <class T>
class Singleton {
public:
  static  T& getInstance() {
      T *instance = sInstance;
      std::unique_lock<std::mutex> lock(sLock);
      if (instance == nullptr) {
          instance = new T();
          sInstance = instance;
      }
      return *sInstance;
  }

protected:
    Singleton(){}
    ~Singleton(){}

private:
    Singleton(const Singleton&);
    Singleton& operator = (const Singleton&);
    static std::mutex sLock;
    static T* sInstance;
};

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#ifndef ANDROID_SINGLETON_STATIC_INSTANCE
#define ANDROID_SINGLETON_STATIC_INSTANCE(TYPE)                 \
    template<typename TYPE> std::mutex Singleton<TYPE>::sLock; \
    template<> TYPE* Singleton< TYPE >::sInstance(nullptr);   \
    template class Singleton< TYPE >;
#endif
}
#endif
