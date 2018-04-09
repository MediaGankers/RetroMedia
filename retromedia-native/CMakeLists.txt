# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(local_src "${CMAKE_SOURCE_DIR}/src")
set(ffmpeg_inc "${local_src}/main/prebuilt/include/ffmpeg-header")
set(yuv_inc "${local_src}/main/prebuilt/include/yuv")

set(libs "${CMAKE_SOURCE_DIR}/src/main/prebuilt/libs")
set(dynamic_prebuilt  "${libs}/shared/${ANDROID_ABI}/")
set(CC_IGNORE_OPTIONS "-Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-unused-private-field")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG -frtti -fexceptions -std=c++11 -g -fpermissive -pedantic -D__STDC_FORMAT_MACROS -Wall ${CC_IGNORE_OPTIONS} -Wreturn-type")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Os")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
endif()

#libffmpeg.so

add_library( libffmpeg
             SHARED
             IMPORTED )
set_target_properties( # Specifies the target library.
                       libffmpeg
                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION
                       # Provides the path to the library you want to import.
                       "${dynamic_prebuilt}/libffmpeg.so")

add_library( libyuv
             SHARED
             IMPORTED )

set_target_properties( # Specifies the target library.
                       libyuv
                       # Specifies the parameter you want to define.
                       PROPERTIES IMPORTED_LOCATION
                       # Provides the path to the library you want to import.
                       "${dynamic_prebuilt}/libyuv.so")

add_library( # Sets the name of the library.
             media-native

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/media-native.cpp
             src/main/cpp/common/log.c
             src/main/cpp/VideoSource.cpp
             src/main/cpp/VideoSourceFile.cpp
             src/main/cpp/Decoder.cpp
             src/main/cpp/source/MediaSource.cpp
             src/main/cpp/source/CameraMediaSource.cpp
             src/main/cpp/source/android/CameraMediaSourceOnAndroid.cpp
             src/main/cpp/source/android/JavaDeliver.cpp
             src/main/cpp/source/android/j4a/camera_wrapper.c
             src/main/cpp/source/android/j4a/i_deliver.c
             src/main/cpp/source/android/j4a/buffer.c
             src/main/cpp/source/android/j4a/j4a_base.c
             src/main/cpp/source/TextureDrawer.cpp
             src/main/cpp/common/Reference.cpp
             src/main/cpp/common/MessageQueue.cpp
             src/main/cpp/common/DebugHelper.cpp
             src/main/cpp/common/BufferPool.cpp
             src/main/cpp/common/Buffer.cpp
             src/main/cpp/common/JNIHelper.cpp
             src/main/cpp/common/MetaData.cpp
             src/main/cpp/common/TexBufferPool.cpp
             src/main/cpp/common/VideoBufferPool.cpp
             src/main/cpp/common/gl/Description.cpp
             src/main/cpp/common/gl/GLES10RenderEngine.cpp
             src/main/cpp/common/gl/GLES11RenderEngine.cpp
             src/main/cpp/common/gl/GLES20RenderEngine.cpp
             src/main/cpp/common/gl/GLExtensions.cpp
             src/main/cpp/common/gl/GlUtils.cpp
             src/main/cpp/common/gl/Mesh.cpp
             src/main/cpp/common/gl/Program.cpp
             src/main/cpp/common/gl/ProgramCache.cpp
             src/main/cpp/common/gl/Rect.cpp
             src/main/cpp/common/gl/Region.cpp
             src/main/cpp/common/gl/RenderEngine.cpp
             src/main/cpp/common/gl/Texture.cpp
             src/main/cpp/common/gl/Transform.cpp)

include_directories(
                    ${ffmpeg_inc}
                    ${yuv_inc}
                    ${local_src}/main/cpp/
                    ${local_src}/main/cpp/common
                    ${local_src}/main/cpp/common/gl
                    ${local_src}/main/cpp/source/android
                    )

add_definitions(
                -DGL_GLEXT_PROTOTYPES
                -DEGL_EGLEXT_PROTOTYPES
                -D__STDC_FORMAT_MACROS
                )

add_definitions(
                -Wno-enum-compare
                -Wno-four-char-constants
               )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log  gnu_stl android)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       media-native

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} libffmpeg libyuv GLESv2 EGL GLESv1_CM android)