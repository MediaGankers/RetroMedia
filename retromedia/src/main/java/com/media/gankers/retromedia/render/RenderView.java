package com.media.gankers.retromedia.render;

/**
 * Created by gordon on 2/3/18.
 */

public interface RenderView {
    void bindController(ViewController controller);

    void release();
}
