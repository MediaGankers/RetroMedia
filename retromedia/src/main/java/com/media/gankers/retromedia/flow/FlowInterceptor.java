package com.media.gankers.retromedia.flow;

import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/6/18.
 */

public interface FlowInterceptor {

    void processFrame(VideoFrame frame);
}
