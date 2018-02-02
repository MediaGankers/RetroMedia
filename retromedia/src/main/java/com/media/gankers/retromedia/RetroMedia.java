package com.media.gankers.retromedia;

/**
 * Created by gordon on 2/2/18.
 */

public class RetroMedia {

    private RetroMedia() {
    }

    private static class Holder {
        private static RetroMedia sInstance = new RetroMedia();
    }

    public static RetroMedia getInstance() {
        return Holder.sInstance;
    }
}
