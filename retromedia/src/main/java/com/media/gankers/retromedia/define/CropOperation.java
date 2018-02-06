package com.media.gankers.retromedia.define;

/**
 * Created by gordon on 2/4/18.
 */

public class CropOperation {
    private float xStart = 0F, yStart = 0F, xOffset = 1F, yOffset = 1F;

    public CropOperation(float xStart, float yStart, float xOffset, float yOffset) {
        this.xStart = xStart;
        this.yStart = yStart;
        this.xOffset = xOffset;
        this.yOffset = yOffset;
    }

    public CropOperation() {
    }

    public float getX() {
        return xStart;
    }

    public float getY() {
        return yStart;
    }

    public float getXOffset() {
        return xOffset;
    }

    public float getYOffset() {
        return yOffset;
    }
}
