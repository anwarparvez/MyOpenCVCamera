/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include<vector>
#include <jni.h>
#include <android/log.h>
#include "cv.h"
#include "cxcore.h"
#include "bmpfmt.h"
#include "com_example_opencvtest_OpenCV.h"
using namespace std;
using namespace cv;
#define ANDROID_LOG_VERBOSE ANDROID_LOG_DEBUG
#define LOG_TAG "TestCVJNI"
#define LOGV(...) __android_log_print(ANDROID_LOG_SILENT, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#ifdef __cplusplus
extern "C" {
#endif
Mat matImage;
Mat *imageMat = NULL;
IplImage* iplImage = NULL;
IplImage* loadPixels2(int* pixels, int width, int height);
IplImage* getIplImageFromIntArray2(JNIEnv* env, jintArray array_data,
		jint width, jint height);

Mat* loadPixels2Mat(int* pixels, int width, int height) {
	int x, y;
	Mat myuv(height, width, CV_8UC3);
	Mat *img = &myuv;
	unsigned char* base = (unsigned char*) (img->data);
	unsigned char* ptr;
	for (y = 0; y < height; y++) {
		ptr = base + y * img->step;
		for (x = 0; x < width; x++) {
			// blue
			ptr[3 * x] = pixels[x + y * width] & 0xFF;
			// green
			ptr[3 * x + 1] = pixels[x + y * width] >> 8 & 0xFF;
			// blue
			ptr[3 * x + 2] = pixels[x + y * width] >> 16 & 0xFF;
		}
	}
	return img;
}
Mat * getMatImageFromIntArray(JNIEnv* env, jintArray array_data, jint width,
		jint height) {

	/*int *pixels = env->GetIntArrayElements(array_data, 0);

	 if (pixels == 0) {
	 LOGE("Error getting int array of pixels.");
	 return 0;
	 }
	 Mat *image = loadPixels2Mat(pixels, width, height);
	 env->ReleaseIntArrayElements(array_data, pixels, 0);
	 if (image == 0) {
	 LOGE("Error loading pixel array.");
	 return 0;
	 }


	 return image;*/

	int *pixels = env->GetIntArrayElements(array_data, 0);

	if (pixels == 0) {
		LOGE("Error getting int array of pixels.");
		return 0;
	}
	IplImage *image = loadPixels2(pixels, width, height);
	env->ReleaseIntArrayElements(array_data, pixels, 0);
	if (image == 0) {
		LOGE("Error loading pixel array.");
		return 0;
	}
	Mat matImg(image);
	Mat *imageMat = &matImg;
	return imageMat;
}

/*
 * Class:     com_example_opencvtest_TestCV
 * Method:    setSourceImage
 * Signature: ([III)Z
 */
JNIEXPORT jboolean JNICALL Java_com_example_opencvtest_TestCV_setSourceImage(
		JNIEnv* env, jobject thiz, jintArray photo_data, jint width,
		jint height) {

	/*	if (imageMat != NULL) {
	 // cvReleaseImage(&iplImage);
	 imageMat = NULL;
	 }

	 imageMat = getMatImageFromIntArray(env, photo_data, width, height);
	 if (imageMat == NULL) {
	 return 0;
	 }

	 LOGI("Load Image Done.");*/

	jint* _bgra = env->GetIntArrayElements(photo_data, 0);

	//Mat A(height, width, CV_8UC3, (unsigned char *) _bgra);
	Mat A(height, width, CV_8UC3);
	for (int i = 0; i < A.rows; i++) {
		for (int j = 0; j < A.cols; j++) {
			A.data[A.step[0] * i + A.step[1] * j + 0] =_bgra[(A.rows-i-1)* A.cols + j + 0]& 0xFF;
			A.data[A.step[0] * i + A.step[1] * j + 1] =_bgra[(A.rows-i-1) * A.cols + j + 0]>> 8 & 0xFF;
			A.data[A.step[0] * i + A.step[1] * j + 2] =_bgra[(A.rows-i-1) * A.cols + j + 0]>> 16 & 0xFF;
		}
	}
	matImage = A;
	rectangle(matImage, cvPoint(20, 15), cvPoint(100, 70), cvScalar(255, 0, 0, 0),1, 8, 0);
	imageMat = &A;

	/*	iplImage =getIplImageFromIntArray2(env, photo_data, width, height);
	 Mat matImg(iplImage);
	 matImage =  matImg;
	 imageMat=&matImage;*/
	//LOGI("Load Image Done.");
	return 1;
}

/*
 * Class:     com_example_opencvtest_TestCV
 * Method:    getSourceImage
 * Signature: ()[B
 */
JNIEXPORT jbyteArray JNICALL Java_com_example_opencvtest_TestCV_getSourceImage(
		JNIEnv* env, jobject thiz) {

	int height = matImage.rows;
	int width = matImage.cols;
	int rowStep = matImage.step;
	int headerSize = 54;
	int imageSize = rowStep * height;
	int fileSize = headerSize + imageSize;
	unsigned char* image = new unsigned char[fileSize];
	struct bmpfile_header* fileHeader = (struct bmpfile_header*) (image);
	fileHeader->magic[0] = 'B';
	fileHeader->magic[1] = 'M';
	fileHeader->filesz = fileSize;
	if (fileHeader->filesz == 0) {
		LOGE("Error in image file size\n");
		return 0;

	}
	fileHeader->creator1 = 0;
	fileHeader->creator2 = 0;
	fileHeader->bmp_offset = 54;
	struct bmp_dib_v3_header_t* imageHeader =
			(struct bmp_dib_v3_header_t*) (image + 14);
	imageHeader->header_sz = 40;
	imageHeader->width = width;
	imageHeader->height = height;
	imageHeader->nplanes = 1;
	imageHeader->bitspp = 24;
	imageHeader->compress_type = 0;
	imageHeader->bmp_bytesz = imageSize;
	imageHeader->hres = 0;
	imageHeader->vres = 0;
	imageHeader->ncolors = 0;
	imageHeader->nimpcolors = 0;

	memcpy(image + 54, matImage.data, imageSize);

	jbyteArray bytes = env->NewByteArray(fileSize);
	if (bytes == 0) {
		LOGE("Error in creating the image.");
		delete[] image;
		return 0;
	}
	env->SetByteArrayRegion(bytes, 0, fileSize, (jbyte*) image);
	delete[] image;
	return bytes;
	/*int width = imageMat->cols;
	 int height = imageMat->rows;
	 int rowStep = imageMat->step;
	 int headerSize = 54;
	 int imageSize = rowStep * height;
	 int fileSize = headerSize + imageSize;
	 unsigned char* image = new unsigned char[fileSize];
	 struct bmpfile_header* fileHeader = (struct bmpfile_header*) (image);
	 fileHeader->magic[0] = 'B';
	 fileHeader->magic[1] = 'M';
	 fileHeader->filesz = fileSize;
	 if (fileHeader->filesz == 0) {
	 LOGE("Error in image file size\n");
	 return 0;

	 }
	 fileHeader->creator1 = 0;
	 fileHeader->creator2 = 0;
	 fileHeader->bmp_offset = 54;
	 struct bmp_dib_v3_header_t* imageHeader =
	 (struct bmp_dib_v3_header_t*) (image + 14);
	 imageHeader->header_sz = 40;
	 imageHeader->width = width;
	 imageHeader->height = height;
	 imageHeader->nplanes = 1;
	 imageHeader->bitspp = 24;
	 imageHeader->compress_type = 0;
	 imageHeader->bmp_bytesz = imageSize;
	 imageHeader->hres = 0;
	 imageHeader->vres = 0;
	 imageHeader->ncolors = 0;
	 imageHeader->nimpcolors = 0;

	 memcpy(image + 54, imageMat->data, imageSize);

	 jbyteArray bytes = env->NewByteArray(fileSize);
	 if (bytes == 0) {
	 LOGE("Error in creating the image.");
	 delete[] image;
	 return 0;
	 }
	 env->SetByteArrayRegion(bytes, 0, fileSize, (jbyte*) image);
	 delete[] image;
	 return bytes;*/

}

/*
 * Class:     com_example_opencvtest_TestCV
 * Method:    extractSURFFeature
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_opencvtest_TestCV_extractSURFFeature
(JNIEnv *, jobject) {

}

/*
 * Class:     com_example_opencvtest_TestCV
 * Method:    findEdgesandCorners
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_opencvtest_TestCV_findEdgesandCorners
(JNIEnv *, jobject) {

}

IplImage* loadPixels2(int* pixels, int width, int height) {
	int x, y;
	IplImage *img = cvCreateImage(cvSize(height,width), IPL_DEPTH_8U, 3);

	unsigned char* base = (unsigned char*) (img->imageData);
	unsigned char* ptr;
	for (y = 0; y < height; y++) {
		ptr = base + y * img->widthStep;
		for (x = 0; x < width; x++) {
			// blue
			ptr[3 * x] = pixels[x + y * width] & 0xFF;
			// green
			ptr[3 * x + 1] = pixels[x + y * width] >> 8 & 0xFF;
			// blue
			ptr[3 * x + 2] = pixels[x + y * width] >> 16 & 0xFF;
		}
	}
	return img;
}

IplImage* getIplImageFromIntArray2(JNIEnv* env, jintArray array_data,
		jint width, jint height) {

	//cv::Mat imageWithData = cv::Mat(cvSize(width, height), 1, CV_8U, NULL).clone();

	//jbyte* _yuv  = env->GetByteArrayElements(yuv, 0);

	/*	 Mat mbgra(height, width, CV_8UC4, (unsigned char *)_bgra);
	 Mat mgray(height, width, CV_8UC1, (unsigned char *)_yuv);*/

	int *pixels = env->GetIntArrayElements(array_data, 0);

	if (pixels == 0) {
		LOGE("Error getting int array of pixels.");
		return 0;
	}
	IplImage *image = loadPixels2(pixels, width, height);
	env->ReleaseIntArrayElements(array_data, pixels, 0);
	if (image == 0) {
		LOGE("Error loading pixel array.");
		return 0;
	}
	return image;
}
#ifdef __cplusplus
}
#endif

