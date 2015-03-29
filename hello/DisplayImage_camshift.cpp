// #include "opencv2/imgcodecs.hpp"
// #include "opencv2/highgui.hpp"
// #include "opencv2/imgproc.hpp"
// #include "opencv2/core/utility.hpp"

// #include "opencv2/highgui/highgui_c.h"

// #include <stdio.h>

// int main() {

// 	cv::Mat image = imread("lena.jpg");

// 	cv::namedWindow("My Image", CV_WINDOW_AUTOSIZE);
// 	cv::imshow("My Image", image);
// 	cv::waitKey(5000);

// 	return 0;
// }
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"

#include "opencv2/highgui/highgui_c.h"
#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"


#include <stdio.h>
#include <stdlib.h> 
#include <iostream>

using namespace std;
using namespace cv;

class ColorHistogtam {
private:
	int histSize[3];
	float hranges[2];
	const float* ranges[3];
	int channels[3];
public:
	ColorHistogtam() {
		histSize[0] = histSize[1] = histSize[2] = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;

		channels[0] = 0;
		channels[1] = 1;
		channels[2] = 2;
	}

	MatND getHistogram(const Mat &image) {
		MatND hist;
		calcHist(&image,
			1,
			channels,
			Mat(),
			hist,
			3,
			histSize,
			ranges
			);
		return hist;
	}

	SparseMat getSparseHistogram(Mat &image) {
		SparseMat hist(3, histSize,CV_32F);
		calcHist(&image,
			1,
			channels,
			Mat(),
			hist,
			3,
			histSize,
			ranges
			);
		return hist;
	}

	Mat applyLoopUp(const Mat &image, const Mat& lookup) {
		Mat result;
		LUT(image,lookup,result);

		return result;

	}

	Mat stretch(const Mat &image, int minValue = 0) {
		MatND hist = getHistogram(image);
		int imin = 0;
		for (;imin < histSize[0] ; imin++ ) {
			cout<<hist.at<float>(imin)<<endl;
			if(hist.at<float>(imin)> minValue)
				break;
		}

		int imax = histSize[0] -1;
		for (; imax >= 0; imax--) {
			if(hist.at<float>(imax) > minValue)
				break;
		}

		int dim(256);
		Mat lookup(1,
			&dim,
			CV_8U);

		for(int i=0; i<256; i++) {
			if (i<imin) lookup.at<uchar>(i) = 0;
			else if (i > imax) lookup.at<uchar>(i) = 255;

			else lookup.at<uchar>(i) = static_cast<uchar> (
				255.0*(i-imin)/(imax-imin)+0.5);
		}
	Mat result;
	result = applyLoopUp(image,lookup);
	return result;

	}

	Mat equalize(const Mat &image) {
		Mat result;
		equalizeHist(image,result);

		return result;
	}

	MatND getHueHistogram(const Mat &image, int minSaturation = 0) {

	MatND hist;
	Mat hsv;
	cvtColor(image, hsv, CV_BGR2HSV);

	Mat mask;

	if(minSaturation> 0) {
		vector<Mat> v;
		split(hsv,v);

		threshold(v[1], mask, minSaturation, 225,
					THRESH_BINARY);
	}

	hranges[0] = 0.0;
	hranges[1] = 180.0;
	channels[0] = 0;

	calcHist(&hsv,
			1,
			channels,
			mask,
			hist,
			1,
			histSize,
			ranges);

	return hist;
}

};

class ContentFinder {
private:
	float hranges[2];
	const float* ranges[3];
	int channels[3];
	float thres;
	MatND histogram;

public:
	ContentFinder(): thres(-1.0f) {
		ranges[0] = hranges;
		ranges[1] = hranges;
		ranges[2] = hranges;
 	}

 	void setThreshold(float t) {
 		thres = t;
 	}

 	float getThreshold() {

 		return thres;
 	}

 	void setHistogram(const MatND& h) {
 		histogram = h;
 		normalize(histogram, histogram,1.0);
 	}

 	Mat find(const Mat& image,
 			float minValue, float maxValue,
 			int *channels, int dim) {
 		Mat result;
 		hranges[0] = minValue;
 		hranges[1] = maxValue;

 		for (int i = 0; i < dim; ++i) {
 			this->channels[i] = channels[i];// why use this->?
 			calcBackProject(&image, 1,
 				channels,
 				histogram,
 				result,
 				ranges,
 				255.0);
 		}

 		if (thres>0.0) {
 			threshold(result, result, 255*thres, 255, THRESH_BINARY);
 		}
 		return result;
 	}
};





int main(){


	ColorHistogtam hc;

	Mat image = imread("baboon1.jpg");
	Mat imageROI = image(Rect(110, 260,35,40));
	rectangle(image, Rect(110, 260, 35, 40), Scalar(0,0,255));

	Mat window(image, Rect(0, 2*image.rows/5, image.cols, image.rows/2));
	namedWindow("Image");
	imshow("Image", window);

	int minSat = 65;

	MatND colorhist = hc.getHueHistogram(imageROI,minSat);
	
	ContentFinder finder;
	finder.setHistogram(colorhist);
	finder.setThreshold(0.2f);

	Mat hsv;
	cvtColor(image, hsv, CV_BGR2HSV);

	vector<Mat> v;
	split(hsv,v);
	threshold(v[1],v[1], minSat, 255, THRESH_BINARY);
	namedWindow("Satruation mask");
	imshow("Satruation mask", v[1]);



	image = imread("baboon4.jpg");
	Mat window2(image, Rect(0, 2*image.rows/5, image.cols, image.rows /2));

	namedWindow("Image 2");
	imshow("Image 2", window2);

	cvtColor(image, hsv, CV_BGR2HSV);

	int ch[1] = {0};
	finder.setThreshold(-1.0f);
	Mat result = finder.find(hsv, 0.0f, 180.0f, ch, 1);

	namedWindow("Backprojection on second image");
	imshow("Backprojection on second image", result);

	Rect rect(110, 260, 35, 40);
	rectangle(image, rect, Scalar(0,0,255));

	TermCriteria criteria(TermCriteria::MAX_ITER,10, 0.01);
	cout <<"meanshif = "<< cv::meanShift(result, rect, criteria)<<endl;

	rectangle(image, rect, Scalar(0, 255, 0));

	Mat window2r(image, Rect(0,2*image.rows/5, image.cols, image.rows/2));

	namedWindow("Image 2 result");
	imshow("image 2 result", window2r);

	waitKey();
}