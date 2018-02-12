package com.media.gankers.sample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

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
    public void deliver(String data) {
        Log.d("deliver", data);
    }

    @Override
    public void onBackPressed() {
        super.onBackPressed();
        mMediaNative.stopWork();
    }
}
