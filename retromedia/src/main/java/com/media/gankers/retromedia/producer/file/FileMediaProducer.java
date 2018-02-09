package com.media.gankers.retromedia.producer.file;

import com.media.gankers.retromedia.define.PlaySpeed;
import com.media.gankers.retromedia.producer.callback.MediaCallback;
import com.media.gankers.retromedia.producer.MediaProducer;
import com.media.gankers.retromedia.producer.callback.SnapCallback;

/**
 * Created by gordon on 2/3/18.
 */

public class FileMediaProducer implements MediaProducer, FileController {

    @Override
    public void openStream(MediaCallback callback) {

    }

    @Override
    public void snapshot(SnapCallback callback) {

    }

    @Override
    public void switchFile(String filePath) {

    }

    @Override
    public void adjustSpeed(PlaySpeed speed) {

    }

    @Override
    public void seek(long time) {

    }

    @Override
    public void pause() {

    }

    @Override
    public void resume() {

    }

    @Override
    public void release() {

    }
}
