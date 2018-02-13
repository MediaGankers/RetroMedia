package com.media.gankers.medianative;

/**
 * Created by gordon on 2/6/18.
 */

public class MediaNative {

    static {
        System.loadLibrary("media-native");
    }

    public native void init();

    public native void startWork();

    public native void stopWork();

    void onData(String data) {
        if (mDeliver != null) {
            mDeliver.deliver(data);
        }
    }

    public void callback(Deliver cb) {
        mDeliver = cb;
    }

    private Deliver mDeliver;

    public interface Deliver {
        void deliver(String data);
    }
}
