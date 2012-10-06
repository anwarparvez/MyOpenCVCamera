package com.example.myopencvcamera;

import org.opencv.android.Utils;
import org.opencv.core.Core;
import org.opencv.core.Mat;
import org.opencv.core.Point;
import org.opencv.core.Scalar;
import org.opencv.highgui.Highgui;
import org.opencv.highgui.VideoCapture;
import org.opencv.imgproc.Imgproc;

import com.example.opencvtest.OpenCV;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.Toast;

class Sample2View extends SampleCvViewBase {
	private Mat mRgba;
	private Mat mGray;
	private Mat mIntermediateMat;
	OpenCV mOpenCV = new OpenCV();

	public Sample2View(Context context) {
		super(context);
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		synchronized (this) {
			// initialize Mats before usage
			mGray = new Mat();
			mRgba = new Mat();
			mIntermediateMat = new Mat();

		}

		super.surfaceCreated(holder);
	}

	public Bitmap getResizedBitmap(Bitmap bm, int newHeight, int newWidth) {
		int width = bm.getWidth();
		int height = bm.getHeight();
		float scaleWidth = ((float) newWidth) / width;
		float scaleHeight = ((float) newHeight) / height;
		// CREATE A MATRIX FOR THE MANIPULATION
		Matrix matrix = new Matrix();
		// RESIZE THE BIT MAP
		matrix.postScale(scaleWidth, scaleHeight);

		// RECREATE THE NEW BITMAP
		Bitmap resizedBitmap = Bitmap.createBitmap(bm, 0, 0, width, height,
				matrix, false);
		return resizedBitmap;
	}

	public Bitmap extractFeature(Bitmap bitmap) {

		bitmap = getResizedBitmap(bitmap, 480, 640);

		Log.i("BitmapSize", "Size" + bitmap.getRowBytes());

		int width = bitmap.getWidth();
		int height = bitmap.getHeight();

		int[] pixels = new int[width * height];
		bitmap.getPixels(pixels, 0, width, 0, 0, width, height);
		// testCV.setSourceImage(pixels, width, height);
		mOpenCV.setSourceImage(pixels, width, height);
		long start = System.currentTimeMillis();
		//mOpenCV.extractSURFFeature();
		 mOpenCV.findEdgesandCorners();
		long end = System.currentTimeMillis();
		// byte[] imageData = testCV.getSourceImage();
		byte[] imageData = mOpenCV.getSourceImage();
		bitmap = BitmapFactory.decodeByteArray(imageData, 0, imageData.length);

		return bitmap;

	}

	@Override
	protected Bitmap processFrame(VideoCapture capture) {
		switch (Sample2NativeCamera.viewMode) {
		case Sample2NativeCamera.VIEW_MODE_GRAY:
			capture.retrieve(mGray, Highgui.CV_CAP_ANDROID_GREY_FRAME);
			Imgproc.cvtColor(mGray, mRgba, Imgproc.COLOR_GRAY2RGBA, 4);
			break;
		case Sample2NativeCamera.VIEW_MODE_RGBA:
			capture.retrieve(mRgba, Highgui.CV_CAP_ANDROID_COLOR_FRAME_RGBA);
			Core.putText(mRgba, "OpenCV + Android", new Point(10, 100), 3, 2,
					new Scalar(255, 0, 0, 255), 3);
			break;
		case Sample2NativeCamera.VIEW_MODE_CANNY:
			capture.retrieve(mGray, Highgui.CV_CAP_ANDROID_GREY_FRAME);
			Imgproc.Canny(mGray, mIntermediateMat, 80, 100);
			Imgproc.cvtColor(mIntermediateMat, mRgba, Imgproc.COLOR_GRAY2BGRA,
					4);
			break;
		}

		Bitmap bmp = Bitmap.createBitmap(mRgba.cols(), mRgba.rows(),
				Bitmap.Config.ARGB_8888);
		

		try {
			Utils.matToBitmap(mRgba, bmp);
			 
			return extractFeature(bmp);
		} catch (Exception e) {
			Log.e("org.opencv.samples.tutorial2",
					"Utils.matToBitmap() throws an exception: "
							+ e.getMessage());
			bmp.recycle();
			return null;
		}
	}

	@Override
	public void run() {
		super.run();

		synchronized (this) {
			// Explicitly deallocate Mats
			if (mRgba != null)
				mRgba.release();
			if (mGray != null)
				mGray.release();
			if (mIntermediateMat != null)
				mIntermediateMat.release();

			mRgba = null;
			mGray = null;
			mIntermediateMat = null;
		}
	}
}
