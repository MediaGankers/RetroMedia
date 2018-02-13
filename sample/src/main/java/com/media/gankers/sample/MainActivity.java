package com.media.gankers.sample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Toast;

import com.media.gankers.medianative.MediaNative;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, MediaNative.Deliver {

    private MediaNative mMediaNative;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        findViewById(R.id.native_test).setOnClickListener(this);
        mMediaNative = new MediaNative();
        mMediaNative.init();
        mMediaNative.callback(this);
    }

    @Override
    public void onClick(View v) {
        mMediaNative.startWork();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mMediaNative.stopWork();
    }

    @Override
    public void deliver(final String data) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(MainActivity.this, data, Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        mMediaNative.stopWork();
    }
}
