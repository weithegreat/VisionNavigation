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

class HarrisDetector {
private:
	Mat cornerStrength;
	Mat cornerTh;
	Mat localMax;
	int neighbourhood;
	int aperture;
	double k;
	double maxStrength;
	double thres;
	int nonMaxSize;
	Mat kernel;

public:
	HarrisDetector() : neighbourhood(3), aperture(3), k(0.01), nonMaxSize(3) {
		setLocalMaxWindowSize(nonMaxSize);
	}

	void detect(const Mat& image) {
		cornerHarris(image, cornerStrength,
			neighbourhood,
			aperture,
			k);

		double minStrenght;
		minMaxLoc(cornerStrength, &minStrenght, &maxStrength);
		Mat dilated;
		dilate(cornerStrength, dilated, Mat());
		compare(cornerStrength, dilated, localMax, CMP_EQ);
	}

	Mat getCornerMap(double qualityLevel) {
		Mat cornerMap;
		thres = qualityLevel*maxStrength;
		threshold(cornerStrength, cornerTh, threshold, 255, THRESH_BINARY);
		cornerTh.convertTo(cornerMap, CV_8U);

		bitwise_and(cornerMap, localMax, cornerMap);

		return cornerMap;
	}

	void getCorners(vector<Point> &points, double qualityLevel) {

		Mat cornerMap = getCornerMap(qualityLevel)
	}

	void getCorners(vector<Point> &points, const Mat& cornerMap) {
		for(int y = 0; y< cornerMap.rows; y++) {
			const uchar* rowPtr = cornerMap.ptr<uchar>(y);
			for(int x = 0; x < cornerMap.cols; x++) {
				if (rowPtr[x]) {
					points.push_back(Point(x,y));
				}
			}
		}
	}

	void drawOnImage(Mat &image, const vector<Point> &points, Scalar color = Scalar(255, 255, 255),
		int radius = 3, int thickness = 2) {

		vector<Point>::const_iterator it = points.begin();

		while (it != points.end()) {
			circle(image, *it, radius, color, thickness);
		}
	}
};


int main() {

	Mat image = imread("church03.jpg");
	cvtColor(image, image, CV_RGB2GRAY);

	HarrisDetector harris;
	harris.detect(image);

	vector<Point> pts;
	harris.getCorners(pts, 0.01);
	harris.drawOnImage(image, pts);

	Mat cornerStrength;
	cornerHarris(image, cornerStrength,
		3,
		3,
		0.01);

	Mat harrisCorners;
	double thres = 0.0001;
	threshold(cornerStrength, harrisCorners, thres, 255, THRESH_BINARY_INV);

	namedWindow("Corner");
	imshow("Corner", harrisCorners);
	waitKey();	
}