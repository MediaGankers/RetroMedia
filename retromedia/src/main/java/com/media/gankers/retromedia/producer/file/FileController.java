package com.media.gankers.retromedia.producer.file;

/**
 * Created by gordon on 2/3/18.
 */

public interface FileController {

    void switchFile(String filePath);

    void seek(long time);

    void pause();

    void resume();

    void release();
}
