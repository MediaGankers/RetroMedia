package com.media.gankers.retromedia.consumer.display.texture;

import android.content.Context;
import android.view.TextureView;

import com.media.gankers.retromedia.consumer.MediaConsumer;
import com.media.gankers.retromedia.consumer.RenderView;

/**
 * Created by gordon on 2/3/18.
 */

public class TextureRenderView extends TextureView implements RenderView {
    private MediaConsumer mConsumer;

    public TextureRenderView(Context context) {
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
