package com.media.gankers.retromedia.render.surface;

import android.content.Context;
import android.opengl.GLSurfaceView;

import com.media.gankers.retromedia.render.RenderView;
import com.media.gankers.retromedia.render.ViewController;

/**
 * Created by gordon on 2/3/18.
 */

public class SurfaceRenderView extends GLSurfaceView implements RenderView {

    private ViewController mController;

    public SurfaceRenderView(Context context) {
        super(context);
    }

    @Override
    public void bindController(ViewController controller) {
        mController = controller;
    }

    @Override
    public void release() {
        mController.release();
    }
}
