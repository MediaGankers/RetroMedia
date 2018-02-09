package com.media.gankers.retromedia.flow.train;

import com.media.gankers.retromedia.flow.FlowInterceptor;

import java.nio.ByteBuffer;

/**
 * Created by gordon on 2/6/18.
 */

public interface TrainInterceptor extends FlowInterceptor {

    void applyToTrain(ByteBuffer nativeBuffer, int width, int height);
}
