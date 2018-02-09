package com.media.gankers.retromedia.consumer.surface;

import android.content.Context;
import android.opengl.GLSurfaceView;

import com.media.gankers.retromedia.consumer.MediaConsumer;
import com.media.gankers.retromedia.consumer.RenderView;

/**
 * Created by gordon on 2/3/18.
 */

public class SurfaceRenderView extends GLSurfaceView implements RenderView {

    private MediaConsumer mConsumer;

    public SurfaceRenderView(Context context) {
        super(context);
    }

    @Override
    public void bindConsumer(MediaConsumer consumer) {
        mConsumer = consumer;
    }

    @Override
    public MediaConsumer getConsumer() {
        return mConsumer;
    }

    @Override
    public void release() {

    }
}
