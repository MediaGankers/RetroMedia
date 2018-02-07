package com.media.gankers.medianative;

import java.nio.ByteBuffer;

/**
 * Created by chao on 2018/2/6.
 */

public class VideoBuffer {
    private long mNativeHandle;

    private VideoBuffer() {
        mNativeHandle = 0;
    }

    public static native VideoBuffer create(int size);
    public native int size();
    public native ByteBuffer byteBuffer();
    private native void release();

    @Override
    protected void finalize() throws Throwable {
        release();
    }
}
