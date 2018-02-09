package com.media.gankers.retromedia.consumer;

/**
 * Created by gordon on 2/3/18.
 */

public interface RenderView {

    void bindConsumer(MediaConsumer controller);

    MediaConsumer getConsumer();

    void release();
}
