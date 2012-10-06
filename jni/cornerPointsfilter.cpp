#include "filters.h"
#include "cv.h"
#include<math.h>
using namespace cv;

bool getLineLenght(CvPoint p1,CvPoint p2){
	return sqrt((double)((p1.x - p2.x)*(p1.x - p2.x)+(p1.y-p2.y)*(p1.y-p2.y))) < 200;
}
bool isVerticalline(vector<CvPoint*> verticalLineholder, CvPoint points){

	double distance1, distance2;
	
	for(int i =0; i < verticalLineholder.size(); i++)
	{
		CvPoint* pointsget = verticalLineholder.at(i);
		if(getLineLenght(pointsget[0],pointsget[1]))
			continue;
		double val1 = (double)((pointsget[0].x - points.x)*(pointsget[0].x - points.x) + (pointsget[0].y - points.y)*(pointsget[0].y - points.y));
		double val2 = (double)((pointsget[1].x - points.x)*(pointsget[1].x - points.x) + (pointsget[1].y - points.y)*(pointsget[1].y - points.y));
		//printf("val1 = %lf val2 = %lf\n",val1,val2);
		distance1  = sqrt(val1);
		distance2  = sqrt(val2);
		//printf("%lf %lf\n",distance1,distance2);
		if(distance1 < 20 || distance2 < 20)
			return true;
	 /* if(pointsget[0].x == points.x && pointsget[0].y == points.y)
		  return true;
      if(pointsget[0].x == points.y && pointsget[0].y == points.x)
		  return true;
	  if(pointsget[1].x == points.x && pointsget[1].y == points.y)
		  return true;
      if(pointsget[1].x == points.y && pointsget[1].y == points.x)
		  return true;*/

	}
	return false;
}
Mat imageRotation(Mat &srcMatrix) {
	
	Mat dstMatrix = cv::Mat(srcMatrix.cols,srcMatrix.rows ,srcMatrix.type());
	cv::Point points1,points2;
	for (int layer = 0; layer < srcMatrix.rows / 2; ++layer) {//one layer is a complete circle
		int first = layer;
		int lastRows = srcMatrix.rows - 1 - layer;
		int lastRowsDst = dstMatrix.rows - 1 - layer;
		int lastColsDst = dstMatrix.cols - 1 - layer;
		int last = srcMatrix.cols - 1 - layer;
		for(int i = first; i < last || i < lastColsDst  ; ++i) {
			 int offset = i - first;
			 points1.y = first;
			 points1.x = i;
			 points2.y = lastRowsDst - offset;
			 points2.x = first;
			 //cv::Vec3b top = srcMatrix.at<cv::Vec3b>(points1); //[first][i]; // save top
			 // left -> bottom
			 if(points2.y > 0 )// checks for whether height is positive
 				dstMatrix.at<cv::Vec3b>(points2) = srcMatrix.at<cv::Vec3b>(points1);
			 if( i < lastRows ){// checks whether the last row of src matrix reaches.
				//left -> top
				points2.x = last;
				points2.y = i;
				dstMatrix.at<cv::Vec3b>(points1) = srcMatrix.at<cv::Vec3b>(points2);
			 }
			 if(last - offset > 0){ // whether the column boundary of src matrix reaches
				 //top -> right
				points1.x = last - offset;
				points1.y = lastRows;
				points2.y = i;
				points2.x = lastColsDst;
				dstMatrix.at<cv::Vec3b>(points2) = srcMatrix.at<cv::Vec3b>(points1);
			 }

			// bottom -> right
			 if(lastRows - offset > 0){// row boundary for source
				 points2.x = first;
				 points2.y = lastRows - offset;
				 points1.y = lastRowsDst;
				 points1.x = lastRows - offset;
				 dstMatrix.at<cv::Vec3b>(points1) = srcMatrix.at<cv::Vec3b>(points2); // right <- saved top
			 }
		}
	}
	imshow("RotatedImage",dstMatrix);
		waitKey(0);
	return dstMatrix;
}
void imageRotationMat(Mat &image,double angle){

	Mat rotationMat = Mat(2,2,CV_64F);
	Mat indexMat = Mat(2,1,CV_64F);
	float xx = cos(angle);
	float xy1 = -sin(angle);
	float xy2 =  sin(angle);
	float yy =   cos(angle);
	rotationMat.at<double>(1,0) = xx;
	rotationMat.at<double>(1,1) = xy1;
	rotationMat.at<double>(0,0) = xy2;
	rotationMat.at<double>(0,1) = yy;
	int newHeight = image.rows * fabs(cos(angle)) + fabs(sin(angle))*image.cols; 
	int newWidth = image.rows * fabs(sin(angle)) + fabs(cos(angle))*image.cols; 
	Mat resultantMat = Mat(newHeight + 15, newWidth + 15,image.type());
	//printf("width = %d height = %d\n",newWidth,newHeight);
	int height1 = image.rows/2;
	int width1 = image.cols/2;
	int resultHeight = resultantMat.rows/2;
	int resultWidth = resultantMat.cols/2;
	for(int i = 0; i < image.rows; i++){
		for(int j = 0; j < image.cols; j++){
			//printf("width = %d height = %d\n",i,j);
			indexMat.at<double>(0,0) = i - height1;
			indexMat.at<double>(1,0) = j - width1;
			Mat tempIndex = rotationMat*indexMat;
			//printf("%lf %lf \n",tempIndex.at<double>(0,0),tempIndex.at<double>(1,0));
			if(tempIndex.at<double>(0,0) - floor(tempIndex.at<double>(0,0)) < 0.5)
				tempIndex.at<double>(0,0) = floor(tempIndex.at<double>(0,0));
			else
				tempIndex.at<double>(0,0) = ceil(tempIndex.at<double>(0,0));

			if(tempIndex.at<double>(1,0) - floor(tempIndex.at<double>(1,0)) < 0.5)
				tempIndex.at<double>(1,0) = floor(tempIndex.at<double>(1,0));
			else
				tempIndex.at<double>(1,0) = ceil(tempIndex.at<double>(1,0));
			resultantMat.at<cv::Vec3b>((int)(tempIndex.at<double>(0,0) + resultHeight),(int)(tempIndex.at<double>(1,0) + resultWidth)) = image.at<cv::Vec3b>(i,j);
		}
		
	}
	Mat kernel = Mat(3,3,CV_32FC1,cv::Scalar(1.0));
	Vec3f sum ;
	sum[0] = sum[1] = sum[2] = 0;
	int counter = 0;
	for(int i = 2; i < resultantMat.cols - 2; i++ )
		for(int j = 2; j < resultantMat.rows - 2; j++ ){
			Vec3b pixel = resultantMat.at<cv::Vec3b>(i,j); 
			if( pixel[0] == 0 && pixel[1] == 0 && pixel[2] == 0 ){
				for(int k = -1 ; k <= 1; k++ )
					for(int l = -1 ; l <= 1; l++){
					  pixel = resultantMat.at<cv::Vec3b>(i + k, j + l);
					  if(pixel[0] != 0 || pixel[1] != 0 || pixel[2] != 0){
					   for(int m = 0 ;m < 3; m++)
					 	 sum[m] += pixel[m]*kernel.at<float>(k+1,l+1);
					   counter++;
					  }
					  
					}
					if(counter >= 4){
						sum[0] = sum[0]/counter;
						sum[1] = sum[1]/counter;
						sum[2] = sum[2]/counter;
				    }
					else
						sum = 0;
				 resultantMat.at<Vec3b>(i,j) = sum;
				 sum[0]= sum[1] = sum[2] = 0;
				 counter = 0;
			}

		}
	//cv::filter2D(resultantMat,resultantMat, -1, kernel,cv::Point(-1,-1));
		imshow("showImage",resultantMat);
		waitKey(0);
	return;
}
vector<CvPoint> getFilteredpoints(CvMat* mat, vector<CvPoint*> verticalLineholder){

	CvScalar harris;
	vector<CvPoint> pointHolder;
	int counter = 0;
	printf("seize of the vertiacal line Holder %d\n",verticalLineholder.size());
	printf("No of Rows %d\n",mat->rows);
	printf("No of Cols %d\n",mat->cols);
   for(int i = 0; i < mat->rows; i++ )
	   for(int j = 0; j < mat->cols; j++){
		  harris = cvGet2D(mat,i,j);
		  if(harris.val[0] > 10e-6){
		    
			counter++;
			if(isVerticalline(verticalLineholder,cvPoint(j,i)))
				pointHolder.push_back(cvPoint(j,i));
		  }
	   }
	   printf("No of times entered the system %d\n",pointHolder.size());
	return pointHolder;
}


bool textureEquality(Mat &image1, Mat &image2){
 int i,j;
	//FOR(i,0, image1.rows) FOR(j,0,image1.cols) ;
	return false;
}

int histogramAnalysis( void)
{

    Mat src = cvLoadImage("C:/Users/Public/Pictures/Sample Pictures/door2.jpg");
    //if( argc != 2 || !(src=imread(argv[1], 1)).data )
      //  return -1;

    Mat hsv;
    cvtColor(src, hsv, CV_BGR2HSV);

    // let's quantize the hue to 30 levels
    // and the saturation to 32 levels
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    // hue varies from 0 to 179, see cvtColor
    float hranges[] = { 0, 180 };
    // saturation varies from 0 (black-gray-white) to
    // 255 (pure spectrum color)
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    MatND hist;
    // we compute the histogram from the 0-th and 1-st channels
    int channels[] = {0, 1};

    calcHist( &hsv, 1, channels, Mat(), // do not use mask
        hist, 2, histSize, ranges,
        true, // the histogram is uniform
        false );
    double maxVal=0;
    minMaxLoc(hist, 0, &maxVal, 0, 0);

    int scale = 10;
    Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

    for( int h = 0; h < hbins; h++ )
        for( int s = 0; s < sbins; s++ )
        {
            float binVal = hist.at<float>(h, s);
            int intensity = cvRound(binVal*255/maxVal);
			cv::rectangle(histImg, Point(h*scale, s*scale),
                         Point( (h+1)*scale - 1, (s+1)*scale - 1),
                         Scalar::all(intensity),
                         CV_FILLED );
        }

    namedWindow( "Source", 1 );
    imshow( "Source", src );

    namedWindow( "H-S Histogram", 1 );
    imshow( "H-S Histogram", histImg );

    waitKey(0);
	return 1;
}


int histogramRGB(void )
{
  Mat src, dst;

 /// Load image
 src = cvLoadImage("C:/Users/Public/Pictures/Sample Pictures/door2.jpg");//imread( argv[1], 1 );

 if( !src.data )
   { return -1; }

 /// Separate the image in 3 places ( R, G and B )
 vector<Mat> rgb_planes;
 split( src, rgb_planes );

 /// Establish the number of bins
 int histSize = 255;

 /// Set the ranges ( for R,G,B) )
 float range[] = { 0, 255 } ;
 int channels[] = {0};
 const float* histRange = { range };

 bool uniform = true; bool accumulate = false;

 MatND r_hist, g_hist, b_hist;

 /// Compute the histograms:
 calcHist( &src, 1, channels, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
 //calcHist( &rgb_planes[1], 1, channels, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
 //calcHist( &rgb_planes[2], 1, channels, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );

 // Draw the histograms for R, G and B
 int hist_w = 400; int hist_h = 400;
 int bin_w = cvRound( (double) hist_w/histSize );

 Mat histImage( hist_w, hist_h, CV_8UC3, Scalar( 0,0,0) );

 /// Normalize the result to [ 0, histImage.rows ]
 normalize(r_hist, r_hist,(double) 0, (double)histImage.rows, NORM_MINMAX, -1, MatND() );
 //normalize(g_hist, g_hist,(double) 0, (double)histImage.rows, NORM_MINMAX, -1, MatND() );
 //normalize(b_hist, b_hist, (double)0, (double)histImage.rows, NORM_MINMAX, -1, MatND() );

 /// Draw for each channel
 for( int i = 1; i < histSize; i++ )
   {
	   cv::rectangle( histImage, Point( bin_w*(i-1), hist_h /*- cvRound(r_hist.at<float>(i-1))*/ ) ,
                      Point( bin_w*(i), hist_h -  cvRound(r_hist.at<float>(i)) ),
                      Scalar( 0, 0, 255), 2, 8, 0  );
     /*line( histImage, Point( bin_w*(i-1), hist_h -  cvRound(g_hist.at<float>(i-1)) ) ,
                      Point( bin_w*(i), hist_h -  cvRound(g_hist.at<float>(i)) ),
                      Scalar( 0, 255, 0), 2, 8, 0  );
     line( histImage, Point( bin_w*(i-1), hist_h -  cvRound(b_hist.at<float>(i-1)) ) ,
                      Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                      Scalar( 255, 0, 0), 2, 8, 0  );*/
    }

 /// Display
 namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE );
 imshow("calcHist Demo", histImage );

 waitKey(0);

 return 0;

}
void cvCreateHistogram(IplImage *image, IplImage *histImage){

	int histSize = 256;
	float range[] = {0,256};
	float *ranges[] = {range};
	float maxVal = 0.0;
	float w_Scale = 0.0;

	CvHistogram* histogram =  cvCreateHist(1,&histSize,CV_HIST_ARRAY,ranges,1);
	cvCalcHist(&image,histogram,0,NULL);
	cvGetMinMaxHistValue(histogram,0,&maxVal,0,0);
	cvScale(histogram->bins,histogram->bins,((float)histImage->height)/maxVal,0);
	w_Scale = ((float)histImage->width)/histSize;

	for(int i =0; i < histSize; i++){
	
		cvRectangle(histImage,cvPoint((int)i*w_Scale,histImage->height),cvPoint((int)(i+1)*w_Scale,histImage->height - cvRound(cvGetReal1D(histogram->bins,i))),cvScalar(0),-1,8,0);
	}

	return;
}