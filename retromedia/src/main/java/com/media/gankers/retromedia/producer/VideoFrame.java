package com.media.gankers.retromedia.producer;

/**
 * Created by gordon on 2/3/18.
 */

public class VideoFrame {
    int format;
    int width;
    int height;
    int stride;
    //可能是buffer数据
    byte[] data;
    //可能是纹理
    int textureId;
}
