package com.media.gankers.retromedia;

import com.media.gankers.retromedia.consumer.MediaConsumer;

import java.util.LinkedHashMap;
import java.util.Map;

/**
 * Created by gordon on 2/2/18.
 */

public class RetroMedia {

    /**
     *
     * */
    private Map<String, MediaConsumer> mMediaConsumers = new LinkedHashMap<>();

    private RetroMedia() {
    }

    private static class Holder {
        private static RetroMedia sInstance = new RetroMedia();
    }

    public static RetroMedia getInstance() {
        return Holder.sInstance;
    }


}
