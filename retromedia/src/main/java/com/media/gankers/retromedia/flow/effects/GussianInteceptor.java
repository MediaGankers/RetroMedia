package com.media.gankers.retromedia.flow.effects;

import android.media.effect.Effect;

import com.media.gankers.retromedia.producer.VideoFrame;

/**
 * Created by gordon on 2/6/18.
 */

public class GussianInteceptor implements EffectInterceptor {

    private Effects mEffect;

    @Override
    public void processFrame(VideoFrame frame) {

    }

    @Override
    public void setEffect(Effects effect) {
        mEffect = effect;
    }
}
