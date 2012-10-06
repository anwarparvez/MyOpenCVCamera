#pragma once
#include "cv.h"
#include "ml.h"
#include <vector>
#include <iostream>
#include <limits>
using namespace std;

void getEdges(Mat& gray, Mat& grayInt, Mat& grayInt1) {
	Mat _tmp,_tmp1,gray32f,res;

	gray.convertTo(gray32f,CV_32FC1,1.0/255.0);

	GaussianBlur(gray32f,gray32f,Size(11,11),0.75);

	Sobel(gray32f,_tmp,-1,1,0,3);	//sobel for dx
	//Sobel(gray32f,_tmp1,-1,1,0,3,-1.0);	//sobel for -dx
	//_tmp = abs(_tmp) + abs(_tmp1);
	//_tmp.copyTo(_tmp,(_tmp > 0.0));
	//_tmp1.copyTo(_tmp1,(_tmp1 > 0.0));
	_tmp1 = abs(_tmp); // + (_tmp1 == 0.0);
	_tmp1.copyTo(res,(_tmp1 > 0.2));
	//res = -res + 1.0;



	double maxVal,minVal;
	minMaxLoc(_tmp,&minVal,&maxVal);

	cv::log(/*(_tmp - minVal) / (maxVal - minVal)*/res,_tmp);
	_tmp = -_tmp * 0.17;
	//_tmp.convertTo(grayInt1,CV_32SC1);
	res.convertTo(grayInt1,CV_32FC1); 
	Sobel(gray32f,_tmp,-1,0,1,3);	//sobel for dy
	//Sobel(gray32f,_tmp1,-1,0,2,3,-1.0);	//sobel for -dy
	//_tmp = abs(_tmp) + abs(_tmp1);
	//_tmp = (_tmp + _tmp1 + 2.0) / 4.0;
	_tmp1 = abs(_tmp);
	 res.setTo(Scalar(0));
	_tmp1.copyTo(res,(_tmp1 > 0.2));
	//res = -res+1.0;
	
	minMaxLoc(_tmp,&minVal,&maxVal);
	cv::log(/*(_tmp - minVal) / (maxVal - minVal)*/res,_tmp);
	
	_tmp = -_tmp * 0.17;
	//_tmp.convertTo(grayInt,CV_32SC1);
	res.convertTo(grayInt,CV_32FC1);
	for(int i =0; i< grayInt.rows; i++){
		float *temp = grayInt.ptr<float>(i);
		for(int j =0 ; j <grayInt.cols; j++)
			temp[j] = fabs(temp[j]);
	}
	
}