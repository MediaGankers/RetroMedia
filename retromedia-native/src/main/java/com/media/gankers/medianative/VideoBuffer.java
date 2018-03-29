package com.media.gankers.medianative;

import java.nio.ByteBuffer;

/**
 * Created by chao on 2018/2/6.
 */

public class VideoBuffer extends Buffer{
    private VideoBuffer(long nativeObj) {
        super(nativeObj);
    }

    public VideoBuffer(VideoBuffer buffer) {
        super(buffer.mNativeObject);
    }

    public native int width();

    public native int height();

    public native long timestamp();

    public native int duration();

    public native int size();

    public native int stride();

    private native int copyToJava(java.nio.Buffer buffer);

    public int copyTo(java.nio.Buffer buffer) {
        if (buffer == null || !buffer.isDirect() || buffer.capacity() < size()) {
            throw new IllegalArgumentException("Buffer is null or must be a direct buffer or buffer is too small");
        }

        return copyToJava(buffer);
    }

    public native byte[] dump();

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
    }

    public native static VideoBuffer create(int w, int h, int timestamp, int duration, int stride, int size);

    public native static VideoBuffer get(Buffer buffer);
}
