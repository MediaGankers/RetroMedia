package com.media.gankers.sample;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Toast;

import com.media.gankers.medianative.MediaNative;

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
    }
}
