package com.media.gankers.retromedia.format;

import android.support.annotation.NonNull;

import com.media.gankers.retromedia.define.CropOperation;

import java.io.File;

/**
 * Created by gordon on 2/3/18.
 */

public class FormatTransformer {

    private Intention mIntention = Intention.MP4_MP4;
    private String mFilePath;
    private String mTargetPath;
    private TransformCallback mCallback;
    private long mStartTime = 0, mEndTime = 0;
    private CropOperation mCrop;

    FormatTransformer(Intention intention, long startTime, long endTime, String filePath,
                      String targetPath, TransformCallback callback,
                      CropOperation operation) {
        this.mIntention = intention;
        this.mStartTime = startTime;
        this.mEndTime = endTime;
        this.mFilePath = filePath;
        this.mTargetPath = targetPath;
        this.mCallback = callback;
        this.mCrop = operation;
    }

    public void startTransform() {
        // TODO: 2/3/18  start transforming
    }

    public void release() {
        // TODO: 2/9/18  release native resources
        this.mCallback = null;
    }

    public static final class Builder {
        Intention intention = Intention.MP4_MP4;
        long startTime = 0;
        long endTime = 0;
        String filePath;
        String targetPath;
        TransformCallback callback;
        CropOperation crop = new CropOperation();

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
            this.crop = new CropOperation(startX, startY, offsetX, offsetY);
            return this;
        }

        public FormatTransformer build() throws Exception {
            if (this.filePath == null) {
                throw new Exception("mFilePath should not be null");
            }
            if (this.targetPath == null) {
                throw new Exception("mTargetPath should not be null");
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
                    this.crop);
        }
    }
}
