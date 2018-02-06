#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_media_gankers_medianative_MediaNative_helloFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from JNI, let's make it awesome";
    return env->NewStringUTF(hello.c_str());
}
