package com.media.gankers.medianative;

/**
 * Created by gordon on 2/6/18.
 */

public class MediaNative {

    static {
        System.loadLibrary("media-native");
        System.loadLibrary("ffmpeg");
        System.loadLibrary("yuv");
    }

    public native String helloFromJNI();
}
