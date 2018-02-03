package com.media.gankers.retromedia.render;

import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/3/18.
 */

public interface ViewController {

    void updateFrame(VideoFrame frame);

    void release();
}
