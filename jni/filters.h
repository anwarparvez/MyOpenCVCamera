#pragma once
#include<stdio.h>
#include<vector>
using namespace std;
#include "cv.h"
#include "highgui.h"
using namespace cv;
#define LS <
#define FOR(k,a,b) for(typeof(a) k = (a); k LS (b); k++)
vector<CvPoint> getFilteredpoints(CvMat* mat, vector<CvPoint*> verticalLineholder);
bool textureEquality(Mat &image1, Mat &image2);
int histogramAnalysis(void);
int histogramRGB(void);
void cvCreateHistogram(IplImage* image,IplImage *histImage);
Mat imageRotation(Mat&);
void imageRotationMat(Mat &image,double angle);