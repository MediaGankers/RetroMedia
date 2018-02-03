package com.media.gankers.retromedia.producer.camera;

import android.hardware.Camera;

/**
 * Created by gordon on 2/3/18.
 */

public class CameraUtils {

    private static final int INVALID_NO = -1;

    private CameraUtils() {
    }

    private static int getDefaultIndex() {
        int numOfCameras = Camera.getNumberOfCameras();
        if (numOfCameras == 0) {
            return INVALID_NO;
        }
        Camera.CameraInfo info = new Camera.CameraInfo();
        for (int i = 0; i < numOfCameras; i++) {
            Camera.getCameraInfo(i, info);
            if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
                return i;
            }
        }
        return INVALID_NO;
    }

    public static Camera getDefaultCamera() {
        return Camera.open(getDefaultIndex());
    }
}
