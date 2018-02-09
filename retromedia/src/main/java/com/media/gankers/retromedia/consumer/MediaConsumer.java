package com.media.gankers.retromedia.consumer;

import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/3/18.
 */

public interface MediaConsumer {

    void consumeFrame(VideoFrame frame);
}
