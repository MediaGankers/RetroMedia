package com.media.gankers.medianative;

/**
 * Created by chao on 2018/3/23.
 */

public class Buffer {
    protected long mNativeObject;

    public Buffer(long nativeObj) {
        mNativeObject = nativeObj;
        addRef();
    }

    public Buffer(Buffer buffer) {
        mNativeObject = buffer.mNativeObject;
        addRef();
    }

    @Override
    protected void finalize() throws Throwable {
        if (mNativeObject != 0) {
            decRef();
        }
        super.finalize();
    }

    public void release() {
        decRef();
        mNativeObject = 0;
    }

    private native void addRef();
    private native void decRef();
}
