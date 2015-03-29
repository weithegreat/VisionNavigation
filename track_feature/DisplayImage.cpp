
// #include "opencv2/imgcodecs.hpp"
// #include "opencv2/highgui.hpp"
// #include "opencv2/imgproc.hpp"
// #include "opencv2/core/utility.hpp"
// #include "opencv2/core/core.hpp"

// #include "opencv2/highgui/highgui_c.h"
// #include "opencv2/videoio.hpp"
// #include "opencv2/video/tracking.hpp"
// #include <vector>

// #include "opencv2/features2d/features2d.hpp"
// #include "opencv2/calib3d/calib3d.hpp"


// #include <stdio.h>
// #include <stdlib.h> 
// #include <iostream>
// #include <iomanip> 


// using namespace std;
// using namespace cv;

// void maxMovementLK(Mat& prev_frame, Mat& frame){
// 	vector<Point2f> initial_features;
// 	goodFeaturesToTrack(prev_frame, initial_features, MAX_FEATURES, 0.1, 0.2);
// 	vector<Point2f>new_features;
// 	vector<uchar>status;
// 	vector<float> err;
// 	TermCriteria criteria(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
// 	Size window(10, 10);
// 	int max_level = 3;
// 	int flags = 0;

// 	double min_eigT = 0.004;

// 	calcOpticalFlowPyrLK(prev_frame, frame, initial_features, 
// 		new_features, status, err, window, max_level, criteria, flags, min_eigT);

// 	double max_move = 0;
// 	double movement = 0;
// 	for (int i=0; i<initial_features.size(); i++) {
// 		Point pointA (initial_features[i].x, initial_features[i].y);
// 		Point pointB (new_features[i].x, new_features[i].y);

// 		line(prev_frame, pointA, pointB, Scalar(255, 0, 0), 2);

// 		movement = norm(pointA-pointB);
// 		if(movement > max_move)
// 			max_move = movement;
// 	}

// 	if(max_move > MAX_MOVEMENT){
// 		putText(prev_frame, "INTRUDER", Point(100, 100), FONT_ITALIC, 3, Scalar(255,0,0), 5);
// 		imshow("Video Camera", prev_frame);
// 		cout<<"Press a key to continue..." <<endl;
// 		waitKey();
// 	}



// }

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

const int MAX_FEATURES = 500;
const int MAX_MOVEMENT = 1000;

void maxMovementLK(Mat& prev_frame, Mat& frame)
{
    // 1-Detect right features to apply the optical flow
    vector<Point2f> initial_features;
    goodFeaturesToTrack(prev_frame, initial_features, MAX_FEATURES, 0.1, 0.2 );
    // cout<<initial_features[1].x<<endl;
    // 2-Set the parameters
    vector<Point2f> new_features;
    vector<uchar> status;
    vector<float> err;
    TermCriteria criteria(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
    Size window(10,10);
    int max_level = 3;
    int flags = 0;
    double min_eigT = 0.004;

    // 3-Lucas-Kanade method for optical flow
    calcOpticalFlowPyrLK(prev_frame, frame, initial_features, new_features, status, err, window, max_level, criteria, flags, min_eigT );

    // 4-Show the results
    double max_move = 0;
    double movement = 0;
    for(int i=0; i<initial_features.size(); i++)
    {
        Point pointA (initial_features[i].x, initial_features[i].y);
        Point pointB(new_features[i].x, new_features[i].y);
        line(prev_frame, pointA, pointB, Scalar(255,0,0), 2);

        movement = norm(pointA-pointB);
        if(movement > max_move)
            max_move = movement;
    }
    if(max_move > MAX_MOVEMENT)
    {
        putText(prev_frame,"INTRUDER",Point(100,100),FONT_ITALIC,3,Scalar(255,0,0),5);
        imshow("Video Camera", prev_frame);
        cout << "Press a key to continue..." << endl;
        waitKey();
    }
}

int videoCamera()
{
    // Open the video camera
    VideoCapture capture(0);

    // Check if video camera is opened
    if(!capture.isOpened()) return 1;

    bool finish = false;
    Mat frame;
    Mat prev_frame;
    namedWindow("Video Camera");

    if(!capture.read(prev_frame)) return 1;

    // Convernto to gray image
    cvtColor(prev_frame, prev_frame, COLOR_BGR2GRAY);

    while(!finish){
        // Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // Convernto to gray image
        cvtColor(frame,frame,COLOR_BGR2GRAY);

        // Detect Maximum Movement with Lucas-Kanade Method
        maxMovementLK(prev_frame, frame);

        imshow("Video Camera", prev_frame);

        // Press Esc to finish
        if(waitKey(1)==27) finish = true;

        prev_frame = frame;
    }
    // Release the video camera
    capture.release();
    return 0;
}

int main( )
{
    videoCamera();
}
