package com.media.gankers.medianative;

import android.annotation.TargetApi;
import android.opengl.GLES20;
import android.os.Build;
import android.os.Looper;

/**
 * Created by chao on 2018/4/4.
 */

class OpenGlEnv implements Runnable{
    private Looper mLoop;
    private Thread mThread;

    public OpenGlEnv() {
        Thread mThread = new Thread(this);
        mThread.setName("GlUtilsThread");
        mThread.start();
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN_MR2)
    public void requestExit() {
        if (mLoop != null) {
            mLoop.quitSafely();
            if (mThread != null) {
                try {
                    mThread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            mLoop = null;
        }
    }

    Looper getLoop() {
        return mLoop;
    }

    private native void initGlEnv();

    private native void deInitGlEnv();

    @Override
    public void run() {
        //initGlEnv();
        Looper.prepare();
        mLoop = Looper.myLooper();
        Looper.loop();
        //deInitGlEnv();
    }
}
