package com.media.gankers.retromedia.flow.canvas;

import android.graphics.Canvas;
import android.graphics.Paint;
import android.support.annotation.NonNull;

import com.media.gankers.retromedia.flow.FlowInterceptor;

/**
 * Created by gordon on 2/6/18.
 */

public interface CanvasInterceptor extends FlowInterceptor {

    void customDraw(@NonNull Canvas canvas, int width, int height, Paint defaultPaint);
}
