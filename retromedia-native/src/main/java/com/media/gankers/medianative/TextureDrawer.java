package com.media.gankers.medianative;

import android.view.Surface;

/**
 * Created by chao on 2018/4/4.
 */

class TextureDrawer {
    private long mNativeObject;
    private long mNativeWindowSurface;

    public TextureDrawer() {
        mNativeObject = 0;
        mNativeWindowSurface = 0;
        initNative();
    }

    public native int makeCurrent(Surface obj);

    public native int draw(Buffer obj);

    public native int updateWindow(int arg1, int arg2);

    public native void release();

    private native void initNative();
}

