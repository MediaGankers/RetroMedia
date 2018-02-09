package com.media.gankers.retromedia.producer.callback;

import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/3/18.
 */

public interface MediaCallback {
    void onFrameProduced(VideoFrame frame);
}
