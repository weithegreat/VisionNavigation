#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
#include <vector>

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"
// #include "opencv2/nonfree/features2d.hpp"

#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <iomanip> 
#include <vector>



#define PI 3.1415926

using namespace std;
using namespace cv;

// Criteria numbers
const int MAX_FEATURES = 500;
vector<Point2f> pointsToTrack;
vector<Vec2f> linesToTrack;
// vector<Point2f> new_points;
double thres = 200;
float alpha = 0.04;
int blockSize = 3;

const int camnum = 0;
const float critNum = 1e-6;
const int minPointNum = 3;
const float lineDev = 0.03;
const int HoughLineCriteria = 80;
const int apertureSize = 3;
const int max_level = 3;
const int flags = 0;
const double min_eigT = 0.01;
const Size subPixWinSize(10,10);
TermCriteria criteria(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);


vector<Point2f> temp;