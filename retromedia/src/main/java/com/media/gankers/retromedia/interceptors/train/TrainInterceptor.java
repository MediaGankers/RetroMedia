package com.media.gankers.retromedia.interceptors.train;

import com.media.gankers.retromedia.interceptors.Interceptor;

import java.nio.ByteBuffer;

/**
 * Created by gordon on 2/6/18.
 */

public interface TrainInterceptor extends Interceptor {

    void applyToTrain(ByteBuffer nativeBuffer, int width, int height);
}
