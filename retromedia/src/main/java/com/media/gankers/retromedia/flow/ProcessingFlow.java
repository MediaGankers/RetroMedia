package com.media.gankers.retromedia.flow;

import android.support.annotation.NonNull;

import com.media.gankers.retromedia.flow.algo.AlgorithmInterceptor;
import com.media.gankers.retromedia.flow.canvas.CanvasInterceptor;
import com.media.gankers.retromedia.flow.effects.EffectInterceptor;

import java.util.LinkedList;
import java.util.List;

/**
 * Created by gordon on 2/6/18.
 */

public class ProcessingFlow {
    private List<FlowInterceptor> mFlowInterceptors = new LinkedList<>();

    public ProcessingFlow addCanvasFlow(@NonNull CanvasInterceptor interceptor) {
        mFlowInterceptors.add(interceptor);
        return this;
    }

    public ProcessingFlow addEffectFlow(@NonNull EffectInterceptor interceptor) {
        mFlowInterceptors.add(interceptor);
        return this;
    }

    public ProcessingFlow addAlgorithm(@NonNull AlgorithmInterceptor interceptor) {
        mFlowInterceptors.add(interceptor);
        return this;
    }

    public ProcessingFlow removeInterceptor(@NonNull CanvasInterceptor interceptor) {
        mFlowInterceptors.remove(interceptor);
        return this;
    }
}
