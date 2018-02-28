package com.media.gankers.retromedia.interceptors;

import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/6/18.
 */

public interface Interceptor {

    void processFrame(VideoFrame frame);
}
