package com.media.gankers.retromedia.interceptors.algo;

import com.media.gankers.retromedia.interceptors.Interceptor;

import java.nio.ByteBuffer;

/**
 * Created by gordon on 2/6/18.
 */

public interface AlgorithmInterceptor extends Interceptor {

    void applyAlgorithm(ByteBuffer nativeBuffer, int width, int height);
}
