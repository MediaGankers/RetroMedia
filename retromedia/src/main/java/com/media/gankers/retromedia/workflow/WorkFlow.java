package com.media.gankers.retromedia.workflow;

import android.support.annotation.NonNull;

import com.media.gankers.retromedia.consumer.MediaConsumer;
import com.media.gankers.retromedia.define.WorkFlowType;
import com.media.gankers.retromedia.interceptors.Interceptor;
import com.media.gankers.retromedia.producer.MediaProducer;
import com.media.gankers.retromedia.producer.VideoFrame;
import com.media.gankers.retromedia.producer.callback.MediaCallback;
import com.media.gankers.retromedia.producer.callback.SnapCallback;
import com.media.gankers.retromedia.producer.camera.CameraMediaProvducer;
import com.media.gankers.retromedia.producer.file.FileMediaProducer;
import com.media.gankers.retromedia.producer.network.NetworkMediaProducer;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

/**
 * Created by gordon on 2/13/18.
 */

public class WorkFlow implements MediaCallback {

    public static final String TAG = WorkFlow.class.getCanonicalName();

    public WorkFlow(WorkFlowType type) {
        this.mType = type;
    }

    private WorkFlowType mType;
    private String mResPath;
    private MediaProducer mProducer;
    private List<Interceptor> mInterceptors = new LinkedList<>();
    private List<MediaConsumer> mConsumers = new ArrayList<>();

    public void configureAsCameraProducer(int defaultId) {
        mProducer = new CameraMediaProvducer();
    }

    public void configureAsFileProducer(String filePath) {
        mProducer = new FileMediaProducer();
    }

    public void configureAsNetworkProducer(String uri) {
        mProducer = new NetworkMediaProducer();
    }

    public void snapshot(SnapCallback cb) {
        mProducer.snapshot(cb);
    }

    public void produceStart() {
        mProducer.openStream(this);
    }

    public void addInterceptors(Interceptor interceptor) {
        mInterceptors.add(interceptor);
    }

    public void addConsumer(MediaConsumer consumer) {
        mConsumers.add(consumer);
    }

    public WorkFlow res(@NonNull String resPath) {
        mResPath = resPath;
        return this;
    }

    @Override
    public void onFrameProduced(VideoFrame frame) {
        for (Interceptor mInterceptor : mInterceptors) {
            mInterceptor.processFrame(frame);
        }
        for (MediaConsumer mConsumer : mConsumers) {
            mConsumer.consumeFrame(frame);
        }
    }
}
