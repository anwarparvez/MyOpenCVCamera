package com.example.myopencvcamera;

import android.os.Bundle;
import android.app.Activity;
import android.view.Menu;

public class MyOpenCVCam extends Activity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_my_open_cvcam);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.activity_my_open_cvcam, menu);
        return true;
    }
}
