package com.media.gankers.medianative;

import android.opengl.GLES20;
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

    public void requestExit() {
        if (mLoop != null) {
            mLoop.quit();
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
