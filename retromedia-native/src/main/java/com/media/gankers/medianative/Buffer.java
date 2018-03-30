package com.media.gankers.medianative;

import android.util.AndroidRuntimeException;
import android.util.Log;

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
        if (mNativeObject == 0) {
            throw new AndroidRuntimeException("Invalid buffer " + buffer);
        }
        addRef();
    }

    @Override
    protected void finalize() throws Throwable {
        Log.d("stone","Java finalize nativeObj " + mNativeObject);
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
