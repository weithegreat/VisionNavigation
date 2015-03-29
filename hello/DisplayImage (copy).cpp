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

#include <stdio.h>
#include <stdlib.h> 
#include <iostream>

using namespace std;
using namespace cv;

void salt(Mat &image, int n) {
	for(int k = 0; k < n; k++) {
		int i = rand()%image.cols;
		int j = rand()%image.rows;

		if (image.channels() == 1) {
			image.at<uchar>(j,i) = 225;
		} else if (image.channels() == 3) {
			image.at<Vec3b>(j,i)[0] = 225;
			image.at<Vec3b>(j,i)[1] = 225;
			image.at<Vec3b>(j,i)[2] = 225;
		}
	}
}


void colorReduce(Mat &image, int div = 64){

	int nl = image.rows;
	int nc = image.cols*image.channels();

	for (int j = 0; j < nl; ++j) {
		uchar* data = image.ptr<uchar>(j);

		for (int i = 0; i < nc; ++i) {
			data[i] = data[i]/div*div+div/2;
		}
	}
}

void sharpen(const Mat &image, Mat &result){
	result.create(image.size(), image.type());

	for (int j = 1; j<image.rows-1; ++j)
	{
		const uchar* previous = image.ptr<const uchar>(j-1);
		const uchar* current = image.ptr<const uchar>(j);
		const uchar* next = image.ptr<const uchar>(j+1);

		uchar *output = result.ptr<uchar>(j);

		for (int i = 1; i < image.cols-1; ++i)
		{
			*output++ = saturate_cast<uchar>(
				5*current[i]-current[i-1]
				-current[i+1]-previous[i]-next[i]);				
		}
	}

	result.row(0).setTo(Scalar(0));
	result.row(result.rows-1).setTo(Scalar(0));
	result.col(0).setTo(Scalar(0));
	result.col(result.cols-1).setTo(Scalar(0));
}


class ColorDetector {
private:
	int minDist;
	Vec3b target;
	Mat result;

public:

	ColorDetector() : minDist(100) {
		target[0] = target[1] = target[2] = 0;
				//std::cout<<"Fuck UU\n";
	}

	void setColorDistanceThreshold(int distance){
		if (distance<0)
			distance = 0;
		minDist = distance;
	}

	int getColorDistanceThreshold() const {
		return minDist;
	}

	void setTargetColor(unsigned char red, unsigned char green, unsigned char blue){
		target[2] = red;
		target[1] = green;
		target[0] = blue;

		//std::cout<<target<<endl;
	}

	void setTargetColor(Vec3b color) {
		// std::cout<<"Set Color!!"<<endl;
		target = color;
	}

	Vec3b getTargetColor() const {
		return target;
	}


	int getDistance(const Vec3b& color) const{
		//std::cout<<"getDistance\n";

		return abs(color[0]-target[0])+ abs(color[1]-target[1])+abs(color[2]-target[2]);
	}

	Mat process(const Mat &image){

		result.create(image.rows, image.cols, CV_8U);
		// cout<<__LINE__<<endl;
		Mat_<Vec3b>::const_iterator it = image.begin<Vec3b>();
		// cout<<__LINE__<<endl;
		Mat_<Vec3b>::const_iterator itend = image.end<Vec3b>();
		//cout<<__LINE__<<endl;
		//cout<<*(result.begin<uchar>())<<endl;
		MatIterator_<uchar> itout = result.begin<uchar>();
		//cout<<__LINE__<<endl;
		for(; it != itend; ++it, ++itout){

			if (getDistance(*it) < minDist)
			{
				*itout = 255;
			} else {
				*itout = 0;
			}
		}
		return result;
	}

};

// Mat Test(Mat &image){
// 	Mat ret;
// 	MatIterator_<uchar> it = image.begin<uchar>();
// 	MatIterator_<uchar> itout = ret.begin<uchar>();

// 	for (; it != ; ++i)
// 	{
// 		/* code */
// 	}
// }

int main() {

	ColorDetector cdetect;

	


	Mat image = imread("sky.jpg");
	// Mat_<Vec3b>::const_iterator it = image.begin<Vec3b>();

	if(!image.data)
		return 0;

	// MatIterator_<uchar> it = image.begin<uchar>();

	// cout<<*it<<endl;

	cdetect.setTargetColor(130,190,230);
	namedWindow("result",CV_WINDOW_AUTOSIZE);

	imshow("result", cdetect.process(image));

	waitKey();

	return 0;

	
	}