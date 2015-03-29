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
#include "opencv2/core/core.hpp"

#include "opencv2/highgui/highgui_c.h"
#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"


#include <stdio.h>
#include <stdlib.h> 
#include <iostream>

#define PI 3.1415926

using namespace std;
using namespace cv;

class LineFinder {

private:
	Mat img;

	vector<Vec4i> lines;
	double deltaRho;
	double deltaTheta;

	int minVote;
	double minLength;

	double maxGap;

public:

	LineFinder() : deltaRho(1), deltaTheta(PI/180), minVote(10),
		minLength(0.), maxGap(0.) {}

	void setAccResolution (double dRho, double dTheta) {
		deltaRho = dRho;
		deltaTheta = dTheta;
	}

	void setMinVote(int minv) {
		minVote = minv;
	}

	void setLineLengthAndGap(double length, double gap) {
		minLength = length;
		maxGap = gap;
	}

	vector<Vec4i> findLines(Mat & binary) {

		lines.clear();
		HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote,
			minLength, maxGap);
		return lines;

	}

	void drawDetectedLines(Mat &image, Scalar color= Scalar(255, 255, 255)) {
		vector<Vec4i>::const_iterator it2 = lines.begin();
		while (it2 != lines.end()) {
			Point pt1((*it2)[0], (*it2)[1]);
			Point pt2((*it2)[2], (*it2)[3]);

			line(image, pt1, pt2, color);
			++it2;
		}
	}

};


int main() {

	clock_t start;
	double duration;

	start = clock();
	Mat contours;
	Mat image = imread("chariot.jpg");

	cvtColor(image, image, CV_RGB2GRAY);
	
	GaussianBlur(image, image, Size(5,5), 1.5);
	vector<Vec3f> circles;

	HoughCircles(image, circles, CV_HOUGH_GRADIENT,
		2,
		50,
		200,
		100,
		25, 100);

	vector<Vec3f>::const_iterator itc= circles.begin();
	while (itc != circles.end()) {
		circle(image, 
			Point((*itc)[0], (*itc)[1]),
			(*itc)[2],
			Scalar(255),
			2);

		++itc;
	}

	duration = (clock() - start) /(double) CLOCKS_PER_SEC;

	cout<<"Time is "<<duration<<endl;
	namedWindow("Circle");
	imshow("Circle", image);

	// HoughLines(test, lines, 1, PI/180, 80);


	// namedWindow("result");
	// imshow("result", contours);
	waitKey();	
}