package com.media.gankers.retromedia.render.texture;

import android.content.Context;
import android.view.TextureView;

import com.media.gankers.retromedia.render.RenderView;
import com.media.gankers.retromedia.render.ViewController;

/**
 * Created by gordon on 2/3/18.
 */

public class TextureRenderView extends TextureView implements RenderView {
    private ViewController mController;

    public TextureRenderView(Context context) {
        super(context);
    }

    @Override
    public void bindController(ViewController controller) {
        mController = controller;
    }

    @Override
    public void release() {

    }
}
