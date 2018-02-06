package com.media.gankers.retromedia.flow.effects;

import com.media.gankers.retromedia.flow.FlowInterceptor;
import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/6/18.
 */

public interface EffectInterceptor extends FlowInterceptor {

    int TYPE_NONE = 0;
    int TYPE_GUSSIAN = 1;
    int TYPE_WAVE = 2;

    void customEffect(VideoFrame videoFrame);
}
