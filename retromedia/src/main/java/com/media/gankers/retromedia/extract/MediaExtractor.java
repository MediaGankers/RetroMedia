package com.media.gankers.retromedia.extract;

import com.media.gankers.retromedia.define.CropOperation;

/**
 * Created by gordon on 2/3/18.
 */

public class MediaExtractor {

    private String mSrcFile;
    private ExtractMode mExtractMode;
    private long mInterval = 30;
    private long mFrameCount = 10;
    private CropOperation mCrop;

    public MediaExtractor(String mSrcFile, ExtractMode mExtractMode, long mInterval, long mFrameCount, CropOperation mCrop) {
        this.mSrcFile = mSrcFile;
        this.mExtractMode = mExtractMode;
        this.mInterval = mInterval;
        this.mFrameCount = mFrameCount;
        this.mCrop = mCrop;
    }

    public static final class Builder {
        private String srcFile;
        private ExtractMode extractMode;
        private long interval = 30;
        private long frameCount = 10;
        private CropOperation crop;

        public Builder() {
        }

        public Builder src(String srcPath) {
            this.srcFile = srcPath;
            return this;
        }

        public Builder crop(float startX, float startY, float offsetX, float offsetY) {
            this.crop = new CropOperation(startX, startY, offsetX, offsetY);
            return this;
        }

        public Builder configureAsFixedInterval(long interval) {
            this.extractMode = ExtractMode.FIXED_INTERVAL;
            this.interval = interval;
            return this;
        }

        public Builder configureAsFixedFrame(int frameCount) {
            this.extractMode = ExtractMode.FIXED_FRAME;
            this.frameCount = frameCount;
            return this;
        }

        public MediaExtractor build() {
            return new MediaExtractor(srcFile, extractMode, interval, frameCount, crop);
        }
    }
}
