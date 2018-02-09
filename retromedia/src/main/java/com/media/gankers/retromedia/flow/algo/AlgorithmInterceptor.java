package com.media.gankers.retromedia.flow.algo;

import com.media.gankers.retromedia.flow.FlowInterceptor;

import java.nio.ByteBuffer;

/**
 * Created by gordon on 2/6/18.
 */

public interface AlgorithmInterceptor extends FlowInterceptor {

    void applyAlgorithm(ByteBuffer nativeBuffer, int width, int height);
}
