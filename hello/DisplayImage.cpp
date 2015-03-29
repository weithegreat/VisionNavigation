#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/core/core.hpp"

#include "opencv2/highgui/highgui_c.h"
#include "opencv2/videoio.hpp"
#include "opencv2/video/tracking.hpp"
#include <vector>

#include "opencv2/features2d/features2d.hpp"
#include "opencv2/calib3d/calib3d.hpp"


#include <stdio.h>
#include <stdlib.h> 
#include <iostream>
#include <iomanip> 


using namespace std;
using namespace cv;

void processV(Mat &frame) {
// 	Mat contours;
// 	// Mat grayImage;

	cvtColor(frame, frame, CV_RGB2GRAY);
	GaussianBlur(frame, frame, Size(5,5), 1.5);
	vector<Vec3f> circles;

	HoughCircles(frame, circles,CV_HOUGH_GRADIENT,
		2,
		50,
		200,
		100,
		25, 100);
	
	int radius;
	vector<Vec3f>::const_iterator itc = circles.begin();
	while (itc != circles.end()) {
		Point center((*itc)[0], (*itc)[1]);
		circle(frame,
		 center,
			(*itc)[2],
			Scalar(255),
			2);
		cout<<center.x<<"\t"<<center.y<<"\t"<<(*itc)[2]<<endl;
		// cout<<'('<<center.x<<", "<<center.y<<") "<<"Radius="(*itc)[2]<<endl;
		++itc;
		// radius = cvRound((*itc)[2])

	}

	// float x = static_cast<float>((*itc)[0]);
	// float y = static_cast<float>((*itc)[1]);

	// cout<<"( "<<x<<", "<<y<<" )\n";

}

int main()
{
    VideoCapture cap(1); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "Cannot open the video cam" << endl;
        return -1;
    }

   // double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
   // double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

   //  cout << "Frame size : " << dWidth << " x " << dHeight << endl;

   //  namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    while (1)
    {
        Mat frame;

        // cap.read(frame);
        // cap.read(frame);
        bool bSuccess = cap.read(frame); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             // cout << "Cannot read a frame from video stream" << endl;
             break;
        }

        processV(frame);

        imshow("MyVideo", frame); //show the frame in "MyVideo" window

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            // cout << "esc key is pressed by user" << endl;
            break; 
       }
    }
    return 0;

}
// int main() {
// 	VideoCapture cap(0);
// 	if(!cap.isOpened())
// 		return 1;
// 	// double rate = cap.get(C)

// 	bool stop(false);
// 	Mat frame;
// 	namedWindow("video");
// 	// int delay = 

// 	while(!stop) {
// 		if(!cap.read(frame))
// 			break;
// 		imshow("video", frame);
// 		if(waitKey(30)==27) 
// 			stop = true;
// 	}

// 	cap.release();
// }