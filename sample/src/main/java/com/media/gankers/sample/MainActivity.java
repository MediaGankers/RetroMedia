package com.media.gankers.sample;

import android.os.Bundle;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.media.gankers.medianative.Buffer;
import com.media.gankers.medianative.CameraMediaSource;
import com.media.gankers.medianative.FileSource;
import com.media.gankers.medianative.IDeliver;
import com.media.gankers.medianative.ISource;
import com.media.gankers.medianative.MediaNative;
import com.media.gankers.medianative.VideoBuffer;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.native_test).setOnClickListener(this);
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


         ISource cameraSource = new CameraMediaSource();
         Log.d("stone","status " + cameraSource.status());

         cameraSource.setConfig(ISource.KEY_VIDEO_FPS, 30);
         cameraSource.setConfig(ISource.KEY_VIDEO_WIDTH, 1280);
         cameraSource.setConfig(ISource.KEY_VIDEO_HEIGHT, 720);
         cameraSource.setConfig(CameraMediaSource.KEY_VIDEO_USING_FRONT, 1);
         cameraSource.setConfig(CameraMediaSource.KEY_VIDEO_MIRROR, 1);

         Log.d("stone","status " + cameraSource.status());

        Log.d("stone","preper ret  " + cameraSource.prepare());

        Log.d("stone","start ret  " + cameraSource.start());
        //Log.d("stone","start ret  " + cameraSource.pause());
        //Log.d("stone","start ret  " + cameraSource.release());

    }
}
