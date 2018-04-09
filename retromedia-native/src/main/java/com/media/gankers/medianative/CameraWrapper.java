package com.media.gankers.medianative;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;

import java.security.Policy;

/**
 * Created by chao on 2018/3/26.
 */

public class CameraWrapper {
    private Camera mCamera;
    private SurfaceTexture mSurfaceTexture;
    private long mNativeObject;
    private float[] mMatrix;

    private static int mFrontCamId = -1;
    private static int mBackCamId = -1;
    private static String mErrorMsg = "null";

    private static int mWidth = 1280;
    private static int mHeight = 720;

    public CameraWrapper(long nativeObj) {
        mNativeObject = nativeObj;
        mMatrix = new float[4 * 4];
    }

    public String getExceptionMessage() {
        String err = mErrorMsg;
        mErrorMsg = "null";
        return err;
    }

    public boolean open(int idx) {
        boolean ret = false;
        try {
            mCamera = Camera.open(idx);
            ret = true;
        }catch (Throwable t) {
            mErrorMsg = "(Open camera " + idx + "failed.)";
            mErrorMsg += t.getMessage();
        }
        return ret;
    }

    public static int getNumOfCamera() {
        int ret = -1;
        try {
            int num = Camera.getNumberOfCameras();

            if (num >=0 ) {
                ret = num;
            }

        }catch (Throwable t) {
            mErrorMsg = "(getNumOfCamera failed.)";
            mErrorMsg += t.getMessage();
        }
        return ret;
    }

    public static int getFrontCameraIdx() {
        int ret = -1;

        if (mFrontCamId >= 0) {
            return mFrontCamId;
        }

        try {
            int cnt = getNumOfCamera();

            if (cnt > 0) {
                for (int idx = 0; idx < cnt; idx++) {
                    Camera.CameraInfo info = new Camera.CameraInfo();
                    Camera.getCameraInfo(idx, info);
                    if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                        mFrontCamId = idx;
                        break;
                    }else if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
                        // try it
                        mBackCamId = idx;
                    }
                }
            }
        }catch (Throwable t) {
            mErrorMsg = "(getFrontCameraIdx failed.)";
            mErrorMsg += t.getMessage();
        }

        return mFrontCamId;
    }

    public static int getBackCameraIdx() {
        int ret = -1;

        if (mBackCamId >= 0) {
            return mBackCamId;
        }

        try {
            int cnt = getNumOfCamera();

            if (cnt > 0) {
                for (int idx = 0; idx < cnt; idx++) {
                    Camera.CameraInfo info = new Camera.CameraInfo();
                    Camera.getCameraInfo(idx, info);
                    if (info.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
                        mBackCamId = idx;
                        break;
                    }else if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                        // try it
                        mFrontCamId = idx;
                    }
                }
            }
        }catch (Throwable t) {
            mErrorMsg = "(getFrontCameraIdx failed.)";
            mErrorMsg += t.getMessage();
        }

        return mBackCamId;
    }

    private void frameAvailable(SurfaceTexture t) {
        t.getTransformMatrix(mMatrix);
        nativeFrameAvailable(0, mMatrix);
    }

    private native void  nativeFrameAvailable(long ts, float[] matrix);
    private native void drawFrameWithMatrix(long ts, float[] matrix);

    private long updateImage() {
        long ret = 0;
        if (mSurfaceTexture != null) {
            try {
                mSurfaceTexture.updateTexImage();
                ret = mSurfaceTexture.getTimestamp();
                mSurfaceTexture.getTransformMatrix(mMatrix);

                drawFrameWithMatrix(ret, mMatrix);
            }catch (Throwable t) {
                mErrorMsg = "(updateTexImage failed)";
                mErrorMsg += t.getMessage();
            }
        }
        return 0;
    }

    public boolean startPreview(int tex) {
        boolean ret = false;
        try {

            if (tex > 0) {
                // in MainLoop?
                mSurfaceTexture = new SurfaceTexture(tex);
                mSurfaceTexture.setOnFrameAvailableListener(new SurfaceTexture.OnFrameAvailableListener() {
                    @Override
                    public void onFrameAvailable(SurfaceTexture surfaceTexture) {
                        frameAvailable(surfaceTexture);
                    }
                });

                if (mCamera != null) {
                    Camera.Parameters p =  mCamera.getParameters();
                    p.setPreviewSize(mWidth, mHeight);
                    p.setPreviewFrameRate(30);

                    mCamera.setParameters(p);
                    mCamera.setPreviewTexture(mSurfaceTexture);
                    mCamera.startPreview();
                    ret = true;
                }
            }
        }catch (Throwable t) {
            mErrorMsg = "(startPreview failed tex " + tex + ")";
            mErrorMsg += t.getMessage();
        }
        return ret;
    }

    public void release() {
        if (mCamera != null) {
            try {
                mCamera.setPreviewTexture(null);
                mCamera.stopPreview();
                mCamera.release();
                mNativeObject = 0;
                mCamera = null;
            }catch (Throwable t) {
                //ignore
            }
        }
    }

    @Override
    protected void finalize() throws Throwable {
        release();
        super.finalize();
    }
}
