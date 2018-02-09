package com.media.gankers.retromedia.producer.network;

/**
 * Created by gordon on 2/3/18.
 */

public interface NetworkController {

    void switchURI(String newURI);

    void seek(long time);

    void pause();

    void resume();

    void release();
}
