package com.media.gankers.retromedia.format;

import android.support.annotation.NonNull;

import java.io.File;

/**
 * Created by gordon on 2/3/18.
 */

public class FormatTransformer {

    private Intention intention = Intention.MP4_MP4;
    private String filePath;
    private String targetPath;
    private TransformCallback callback;
    private long startTime = 0, endTime = 0;
    private float xStart = 0F, yStart = 0F, xOffset = 1F, yOffset = 1F;

    FormatTransformer(Intention intention, long startTime, long endTime, String filePath,
                      String targetPath, TransformCallback callback,
                      float startX, float startY, float offsetX, float offsetY) {
        this.intention = intention;
        this.startTime = startTime;
        this.endTime = endTime;
        this.filePath = filePath;
        this.targetPath = targetPath;
        this.callback = callback;
        this.xStart = startX;
        this.yStart = startY;
        this.xOffset = offsetX;
        this.yOffset = offsetY;
    }

    public void startTransform() {
        // TODO: 2/3/18  开始转化
    }

    public void release() {
        this.callback = null;
    }

    public static final class Builder {
        Intention intention = Intention.MP4_MP4;
        long startTime = 0;
        long endTime = 0;
        String filePath;
        String targetPath;
        TransformCallback callback;
        float xStart = 0F, yStart = 0F, xOffset = 1F, yOffset = 1F;

        public Builder() {
        }

        public Builder setTime(long start, long end) {
            this.startTime = start;
            this.endTime = end;
            return this;
        }

        public Builder targetPath(@NonNull String path) {
            this.targetPath = path;
            return this;
        }

        public Builder src(@NonNull String path) {
            this.filePath = path;
            return this;
        }

        public Builder intent(Intention intention) {
            this.intention = intention;
            return this;
        }

        public Builder callback(TransformCallback callback) {
            this.callback = callback;
            return this;
        }

        public Builder crop(float startX, float startY, float offsetX, float offsetY) {
            this.xStart = startX;
            this.yStart = startY;
            this.xOffset = offsetX;
            this.yOffset = offsetY;
            return this;
        }

        public FormatTransformer build() throws Exception {
            if (this.filePath == null) {
                throw new Exception("filePath should not be null");
            }
            if (this.targetPath == null) {
                throw new Exception("targetPath should not be null");
            }
            if (new File(this.filePath).exists()) {
                throw new Exception("src file is not available");
            }
            if (!this.targetPath.endsWith(intention.getSuffix())) {
                this.targetPath = this.targetPath.substring(0,
                        this.targetPath.lastIndexOf('.')) + intention.getSuffix();
            }
            return new FormatTransformer(this.intention, this.startTime, this.endTime,
                    this.filePath, this.targetPath, this.callback,
                    this.xStart, this.yStart, this.xOffset, this.yOffset);
        }
    }
}
