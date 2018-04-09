package com.media.gankers.sample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.Toast;

import com.media.gankers.medianative.Buffer;
import com.media.gankers.medianative.CameraMediaSource;
import com.media.gankers.medianative.IDeliver;
import com.media.gankers.medianative.ISource;
import com.media.gankers.medianative.MediaNative;
import com.media.gankers.medianative.TextureBuffer;
import com.media.gankers.medianative.VideoBuffer;
import com.media.gankers.medianative.WindowRender;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    private WindowRender render;
    private ISource mSource;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        long ms = System.currentTimeMillis();
        Log.d("STONE", "onCreate ++++");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Toast.makeText(this, new MediaNative().helloFromJNI(), Toast.LENGTH_SHORT).show();
        SurfaceView view = (SurfaceView) findViewById(R.id.SurfaceView);
        render = new WindowRender(view);
        findViewById(R.id.native_test).setOnClickListener(this);
        mSource = new CameraMediaSource();
        Log.d("stone","status " + mSource.status());

        mSource.setConfig(ISource.KEY_VIDEO_FPS, 30);
        mSource.setConfig(ISource.KEY_VIDEO_WIDTH, 1280);
        mSource.setConfig(ISource.KEY_VIDEO_HEIGHT, 720);
        mSource.setConfig(CameraMediaSource.KEY_VIDEO_USING_FRONT, 0);
        mSource.setConfig(CameraMediaSource.KEY_VIDEO_MIRROR, 1);
        mSource.addDeliver(render);
        Log.d("STONE", "onCreate --- " + (System.currentTimeMillis() - ms));
    }


    @Override
    protected void onStart() {
        long ms = System.currentTimeMillis();
        Log.d("STONE", "onStart ++++");
        super.onStart();
        mSource.prepare();
        Log.d("STONE", "onStart --- " + (System.currentTimeMillis() - ms));

    }

    @Override
    protected void onPause() {
        long ms = System.currentTimeMillis();
        Log.d("STONE", "onPause ++++");
        super.onPause();
        mSource.pause();
        Log.d("STONE", "onPause --- " + (System.currentTimeMillis() - ms));

    }

    @Override
    protected void onResume() {
        long ms = System.currentTimeMillis();
        Log.d("STONE", "onResume ++++");
        super.onResume();
        mSource.start();
        Log.d("STONE", "onResume --- " + (System.currentTimeMillis() - ms) + "stat " + mSource.status());

    }

    @Override
    protected void onStop() {
        long ms = System.currentTimeMillis();
        Log.d("STONE", "onStop ++++");
        super.onStop();
        mSource.stop();
        Log.d("STONE", "onStop --- " + (System.currentTimeMillis() - ms));

    }

    @Override
    protected void onDestroy() {
        long ms = System.currentTimeMillis();
        Log.d("STONE", "onDestroy ++++");
        super.onDestroy();
        render.destroy();
        mSource.release();
        mSource = null;
        Log.d("STONE", "onDestroy --- " + (System.currentTimeMillis() - ms));
    }

    @Override
    public void onClick(View v) {
        Toast.makeText(this, new MediaNative().helloFromJNI(), Toast.LENGTH_SHORT).show();
        VideoBuffer buffer = VideoBuffer.create(1280, 820, 100, 200, 1300, 1300 * 820);
        Log.d("stone","stone size " + buffer.size());
        Log.d("stone","stone w " + buffer.width());
        Log.d("stone","stone h " + buffer.height());
        Log.d("stone","stone stride " + buffer.stride());
        Log.d("stone","stone timestamp " + buffer.timestamp());
        Log.d("stone","stone duration " + buffer.duration());
        VideoBuffer buffer2 = VideoBuffer.create(1280, 820, 100, 200, 1300, 1300 * 820);
        buffer2.release();

        byte[] data = buffer.dump();
        Log.d("stone","stone byte size " + data.length);
         buffer2 = VideoBuffer.create(1280, 820, 100, 200, 1300, 1300 * 820);
         buffer2 = VideoBuffer.create(1280, 820, 100, 200, 1300, 1300 * 820);
         VideoBuffer bb = new VideoBuffer(buffer2);
         bb.dump();
    }
}
