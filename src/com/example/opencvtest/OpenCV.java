package com.example.opencvtest;

import android.util.Log;

public class OpenCV {
	static{
		System.loadLibrary("main_object");
		Log.i("CV", "Load Library");
	}
	public native boolean setSourceImage(int[] pixels, int width, int height);
	public native byte[] getSourceImage();
	public native void extractSURFFeature();
	public native void findEdgesandCorners();
	
	
}
