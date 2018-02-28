package com.media.gankers.retromedia.interceptors.effects;

import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/6/18.
 */

public class WaveInterceptor implements EffectInterceptor {

    private Effects mEffect;

    @Override
    public void processFrame(VideoFrame frame) {

    }

    @Override
    public void setEffect(Effects effect) {
        mEffect = effect;
    }
}
