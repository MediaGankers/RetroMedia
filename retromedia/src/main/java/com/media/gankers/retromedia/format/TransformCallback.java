package com.media.gankers.retromedia.format;

/**
 * Created by gordon on 2/3/18.
 */

public interface TransformCallback {
    void onProgress(int progress);

    void onFinished(String filePath);
}
