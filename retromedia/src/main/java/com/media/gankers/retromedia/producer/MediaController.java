package com.media.gankers.retromedia.producer;

/**
 * Created by gordon on 2/3/18.
 */

public interface MediaController {

    void seek(long time);

    void pause();

    void resume();

    void release();
}
