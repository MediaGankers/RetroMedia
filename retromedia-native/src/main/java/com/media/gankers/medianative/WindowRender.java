package com.media.gankers.medianative;

import android.graphics.SurfaceTexture;
import android.opengl.GLES20;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.TextureView;

import java.lang.ref.WeakReference;

/**
 * Created by chao on 2018/4/4.
 */

public class WindowRender extends ISink implements SurfaceHolder.Callback {
    private EventHandle mHandler;
    private OpenGlEnv mGlEnv;
    private SurfaceTexture mSurfaceTexture;
    private Surface mSurface;
    private Object mFence = new Object();
    TextureBuffer mTexBuffer;

    TextureDrawer mDrawer;
    private final static int MSG_RENDER_FRAME = 1;
    private final static int MSG_SURFACE_CREATE = 2;
    private final static int MSG_SURFACE_DESTROY = 3;
    private final static int MSG_SURFACE_CHANGE = 4;
    private final static int MSG_SURFACETEXTURE_CREATE = 5;
    private final static int MSG_QUIT = 6;
    private final static int MSG_CREATE_GLRENDER = 7;


    class EventHandle extends Handler {
        private WeakReference<WindowRender> mThis;

        public EventHandle(Looper looper, WeakReference<WindowRender> thiz) {
            super(looper);
            mThis = thiz;
        }

        @Override
        public void handleMessage(Message msg) {
            WindowRender r = mThis.get();

            if (r == null) {
                return;
            }

            switch (msg.what) {
                case MSG_RENDER_FRAME:
                    r.onRenderFrame((Buffer)msg.obj);
                    break;
                case MSG_SURFACE_CREATE:
                    r.onSurfaceCreated((Surface)msg.obj);
                    break;
                case MSG_SURFACE_CHANGE:
                    r.onSurfaceChange(msg.arg1, msg.arg2);
                    break;
                case MSG_SURFACE_DESTROY:
                    r.onSurfaceDestroy();
                    break;
                case MSG_SURFACETEXTURE_CREATE:
                    r.onSurfaceTextureCreate((SurfaceTexture.OnFrameAvailableListener)msg.obj);
                    break;
                case MSG_QUIT:
                    r.onDestroy();
                    break;
                case MSG_CREATE_GLRENDER:
                    r.onCreateGlRender();
                    break;
                default:
                    break;
            }
        }
    }

    private void onCreateGlRender() {
        if (mDrawer == null) {
            mDrawer = new TextureDrawer();
        }
    }

    private void onDestroy() {
        if (mDrawer != null) {
            mDrawer.release();
            mDrawer = null;
        }

        if (mSurfaceTexture != null) {
            mSurfaceTexture.release();
            mSurfaceTexture = null;
        }

        if (mTexBuffer != null) {
            mTexBuffer.release();
            mTexBuffer = null;
        }

        mGlEnv.getLoop().quit();
    }

    private void onSurfaceTextureCreate(SurfaceTexture.OnFrameAvailableListener obj) {
        mTexBuffer = TextureBuffer.create(0, 0, null);
        mSurfaceTexture = new SurfaceTexture(mTexBuffer.textureId());
        mSurfaceTexture.setOnFrameAvailableListener(obj);
        onSurfaceCreated(new Surface(mSurfaceTexture));
    }

    private void onSurfaceDestroy() {
        mDrawer.makeCurrent(null);
    }

    private void onSurfaceChange(int arg1, int arg2) {
        mDrawer.updateWindow(arg1, arg2);
    }

    private void onSurfaceCreated(Surface obj) {

        if (obj != null) {
            mDrawer.makeCurrent(obj);
        }
    }

    private void onRenderFrame(Buffer obj) {
            mDrawer.draw(obj);
            obj.release();
    }

    @Override
    protected void processVideo(Buffer buffer) {
        synchronized (mFence) {
            if (mGlEnv != null && mHandler != null) {
                Message msg = mHandler.obtainMessage(MSG_RENDER_FRAME);
                msg.obj = new Buffer(buffer);
                mHandler.sendMessage(msg);
            }
        }
    }

    @Override
    protected void processAudio(Buffer buffer) {

    }

    private WindowRender() {
        super();
        mGlEnv = new OpenGlEnv();
        while (mGlEnv.getLoop() == null) {
            try {
                Thread.sleep(500);
            } catch (InterruptedException e) {

            }
        }

        mHandler = new EventHandle(mGlEnv.getLoop(), new WeakReference<WindowRender>(this));
        mHandler.sendEmptyMessage(MSG_CREATE_GLRENDER);
    }

    public WindowRender(SurfaceView view) {
        this();
        view.getHolder().addCallback(this);
    }

    public WindowRender(SurfaceTexture.OnFrameAvailableListener listener) {
        this();
        Message msg = mHandler.obtainMessage(MSG_SURFACETEXTURE_CREATE);
        msg.obj = listener;
        mHandler.sendMessage(msg);
    }

    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {
        Message msg = mHandler.obtainMessage(MSG_SURFACE_CREATE);
        msg.obj = surfaceHolder.getSurface();
        mHandler.sendMessage(msg);
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        Message msg = mHandler.obtainMessage(MSG_SURFACE_CHANGE);
        msg.obj = surfaceHolder.getSurface();
        msg.arg1 = i1;
        msg.arg2 = i2;
        mHandler.sendMessage(msg);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
        synchronized (mFence) {
            if (mGlEnv != null) {
                mHandler.sendEmptyMessage(MSG_SURFACE_DESTROY);
            }
        }
    }

    public  void destroy() {
        synchronized (mFence) {
            if (mGlEnv != null) {
                mHandler.sendEmptyMessage(MSG_SURFACE_DESTROY);
                mGlEnv.requestExit();
                mGlEnv = null;
            }
        }
    }

    @Override
    protected void finalize() throws Throwable {
        destroy();
    }
}
