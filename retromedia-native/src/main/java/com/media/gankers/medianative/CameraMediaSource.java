package com.media.gankers.medianative;

import android.os.Bundle;

/**
 * Created by chao on 2018/3/26.
 */

public class CameraMediaSource extends ISource {
    public static final int KEY_VIDEO_USING_FRONT = 'a' << 24 | 'c' << 16 | 'm' << 8 | 'f';
    public static final int KEY_VIDEO_MIRROR = 'a' << 24 | 'c' << 16 | 'm' << 8 | 'M';

    @Override
    public Bundle getSourceInfo() {
        return null;
    }

    @Override
    protected int init() {
       return initNative();
    }

    public void setConfig(int key, Object value) {
        if (filter(key, value)) {
            config(key, value);
        }else {
            throw new IllegalArgumentException("Unsupported key [" + fourCC(key) + "] or value [" + value + "] type on CameraMediaSource");
        }
    }

    private boolean filter(int key, Object ob) {
        boolean filter = false;
        switch (key) {
            case KEY_VIDEO_FPS:
            case KEY_VIDEO_BITRATE:
            case KEY_VIDEO_HEIGHT:
            case KEY_VIDEO_WIDTH:
            case KEY_VIDEO_USING_FRONT:
            case KEY_VIDEO_MIRROR:
                if (ob instanceof Integer) {
                    filter = true;
                    break;
                }
            default:
                break;

        }
        return filter;
    }

    private native int initNative();

    private native int config(int key, Object obj);
}
