package com.media.gankers.retromedia.producer;

import com.media.gankers.retromedia.producer.callback.MediaCallback;
import com.media.gankers.retromedia.producer.callback.SnapCallback;

/**
 * Created by gordon on 2/3/18.
 * 考虑文件格式，作为什么方式被打开，可选项包括
 */

public interface MediaProducer {

    void openStream(MediaCallback callback);

    void snapshot(SnapCallback callback);
}
