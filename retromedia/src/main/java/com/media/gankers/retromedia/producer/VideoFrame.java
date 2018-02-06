package com.media.gankers.retromedia.producer;

import com.media.gankers.retromedia.define.FrameFormat;

import java.nio.ByteBuffer;

/**
 * Created by gordon on 2/3/18.
 */

public class VideoFrame {

    FrameFormat format;
    int width;
    int height;
    int stride;

    //可能是buffer数据
    ByteBuffer[] nativeData;

    //可能是纹理
    ITexture texture;
}
