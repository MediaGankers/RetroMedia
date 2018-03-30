package com.media.gankers.medianative;

import android.app.Activity;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.util.AndroidRuntimeException;

import java.lang.annotation.Native;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by chao on 2018/3/25.
 */

public abstract class ISource{
    /*
* key(int), value(int)
* */
    public static final int KEY_VIDEO_FPS = 'f' << 24 | 'r' << 16 | 'm' << 8 | 'R';
    public static final int KEY_VIDEO_WIDTH = 'w' << 24 | 'i' << 16 | 'd' << 8 | 't';
    public static final int KEY_VIDEO_HEIGHT = 'h' << 24 | 'e' << 16 | 'i' << 8 | 'g';
    public static final int KEY_VIDEO_BITRATE = 'b' << 24 | 'r' << 16 | 't' << 8 | 'e';

    private Handler mHandler;
    private long mNativeObject;
    protected Bundle mBundle;
    protected List<IDeliver> mDelivers;
    protected ISourceListener mListener;
    protected Thread mJniThread;

    private static final int MSG_SOURCE_ERROR = 0x1000;
    private static final int MSG_SOURCE_INFO = 0x1001;
    private static final int MSG_SOURCE_STATUS = 0x1002;
    private static final int MSG_SOURCE_PREPED = 0x1003;

    protected ISource() {
        mDelivers = new ArrayList<>();
        mBundle = new Bundle();
        mNativeObject = 0;
        init();
        mJniThread = new Thread(new Runnable() {
            @Override
            public void run() {
                mJniThread.setName("ISourceThread");
                loop();
            }
        });
    }

    // call by native
    private void onMessage(int what, int arg0, int arg1, Object obj) {
        ISourceListener listener = mListener;

        if (listener == null) {
            return;
        }

        Message msg = new Message();
        msg.what = what;
        msg.arg1 = arg0;
        msg.arg2 = arg1;
        msg.obj = obj;
        switch (msg.what) {
            case MSG_SOURCE_ERROR:
                listener.onError(this, msg);
                break;
            case MSG_SOURCE_INFO:
                listener.onInfo(this, msg);
                break;
            case MSG_SOURCE_PREPED:
                listener.onPreped(this, msg);
                break;
            case MSG_SOURCE_STATUS:
                listener.onStatus(this, msg);
                break;
            default:
                throw new AndroidRuntimeException("Unknown msg " + msg.what + " msg " + msg);
        }
    }

    public native int start();

    public int prepAsync() {
        Thread t = new Thread(new Runnable() {
            @Override
            public void run() {
                prepare();
            }
        });
        t.start();
        return 0;
    }

    public native int prepare();

    protected int init() {
        return -1;
    }

    public native void addDeliver(IDeliver deliver);

    public native int stop();

    public native int release();

    public native int pause();

    private native boolean loop();

    public Bundle getConfig() {
        return mBundle;
    }

    public abstract Bundle getSourceInfo();

    public void setConfig(int key, Object value){

    }

    public native int status();

    public void setListener(ISourceListener listener) {
        mListener = listener;
    }

    public interface ISourceListener {
        void onError(ISource source, Message msg);
        void onInfo(ISource source, Message msg);
        void onPreped(ISource source, Message msg);
        void onStatus(ISource source, Message msg);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        release();
        mJniThread.interrupt();
        mJniThread = null;
    }

    public static final int fourCC(char a, char b, char c, char d) {
        return a << 24 | b << 16 | c << 8 | d;
    }

    public static final String fourCC(int v) {
        String cc = new String();
        cc += (char) ((v >> 24) & 0xFF);
        cc += (char) ((v >> 16) & 0xFF);
        cc += (char) ((v >> 8) & 0xFF);
        cc += (char) (v & 0xFF);
        return cc;
    }
}
