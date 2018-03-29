package com.media.gankers.medianative;

import android.os.Bundle;
import android.os.Message;

import java.net.URL;

/**
 * Created by chao on 2018/3/25.
 */

public class FileSource extends ISource{

    public FileSource(String url) {
        super();
        mBundle.putString("url", url);
        setListener(new ISourceListener() {
            @Override
            public void onError(ISource source, Message msg) {

            }

            @Override
            public void onInfo(ISource source, Message msg) {

            }

            @Override
            public void onPreped(ISource source, Message msg) {

            }

            @Override
            public void onStatus(ISource source, Message msg) {

            }
        });
    }

    @Override
    public Bundle getSourceInfo() {
        return null;
    }

    @Override
    protected int init() {
        return initNative();
    }

    private native int initNative();

    public void setConfig(int key, Object value) {
        if (filter(key, value)) {
            config(key, value);
        }else {
            throw new IllegalArgumentException("Unsupported key [" + fourCC(key) + "] or value [" + value + "] type on FileSource");
        }
    }

    private boolean filter(int key, Object ob) {
        boolean filter = true;
        switch (key) {
            case KEY_VIDEO_FPS:
            case KEY_VIDEO_BITRATE:
            case KEY_VIDEO_HEIGHT:
            case KEY_VIDEO_WIDTH:
                if (ob instanceof Integer) {
                    filter = false;
                    break;
                }
            default:
                break;

        }
        return filter;
    }

    private native int config(int key, Object obj);
}
