package com.media.gankers.medianative;

/**
 * Created by chao on 2018/3/25.
 */

public class TextureBuffer extends Buffer {
    private TextureBuffer(long nativeObj) {
        super(nativeObj);
    }

    public TextureBuffer(TextureBuffer buffer) {
        super(buffer);
    }

    public native int textureId();

    public native int width();

    public native int height();

    public native int textureType();

    public native static TextureBuffer create(int w, int h, byte[] bytes);

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
    }
}
