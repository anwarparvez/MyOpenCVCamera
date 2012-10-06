#include <cv.h>
#include<iostream>
#include<highgui.h>
using namespace cv;

class morphological{
  
public :
	IplImage* opening(IplImage *image,int rows,int cols,int shape,int iter);
	IplImage* closing(IplImage *image,int rows,int cols,int shape,int iter);
};