package com.media.gankers.medianative;

import android.view.TextureView;

/**
 * Created by chao on 2018/4/4.
 */

public abstract class ISink implements IDeliver {
    public final int STREAM_TYPE_IS_VIDEO = 1;
    public final int STREAM_TYPE_IS_AUDIO = 2;

    private IDeliver mNextSink;

    @Override
    public void deliver(Buffer buffer, int type) {
        switch (type) {
            case STREAM_TYPE_IS_VIDEO:
                processVideo(buffer);
                break;
            case STREAM_TYPE_IS_AUDIO:
                processAudio(buffer);
                break;
            default:
                    break;
        }

        if (mNextSink != null) {
            mNextSink.deliver(buffer, type);
        }

        buffer.release();
    }

    public void setNextSink(IDeliver deliver) {
        mNextSink = deliver;
    }

    protected abstract void processVideo(Buffer buffer);
    protected abstract void processAudio(Buffer buffer);
}
