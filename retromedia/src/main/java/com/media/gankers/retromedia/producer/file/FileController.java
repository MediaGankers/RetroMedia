package com.media.gankers.retromedia.producer.file;

import com.media.gankers.retromedia.define.PlaySpeed;

/**
 * Created by gordon on 2/3/18.
 */

public interface FileController {

    void switchFile(String filePath);

    void adjustSpeed(PlaySpeed speed);

    void seek(long time);

    void pause();

    void resume();

    void release();
}
