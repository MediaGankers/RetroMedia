package com.media.gankers.retromedia.format;

/**
 * Created by gordon on 2/3/18.
 */

public enum Intention {
    MP4_MP4("mp4"),

    MP4_WEBP("webp"),

    MP4_FLV("flv"),

    MP4_GIF("gif");

    private String suffix;

    Intention(String suffix) {
        this.suffix = suffix;
    }

    public String getSuffix() {
        return suffix;
    }
}
