#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;


int videoCamera()
{
	TermCriteria criteria(TermCriteria::COUNT | TermCriteria::EPS, 10, 1);

    // 2-Tracking using CamShift
    

    // Open the video camera
    VideoCapture capture(0);
    RotatedRect found_object;
    Rect found_rect;

    // Check if video camera is opened
    if(!capture.isOpened()) return 1;

    bool finish = false;
    Mat frame;
    Mat prev_frame;
    namedWindow("Video Camera");

    if(!capture.read(prev_frame)) return 1;

    // Convernto to gray image
    cvtColor(prev_frame,prev_frame,COLOR_BGR2GRAY);

    Rect search_window(200,150,100,100);

    while(!finish)
    {
        // Read each frame, if possible
        if(!capture.read(frame)) return 1;

        // Convernto to gray image
        cvtColor(frame,frame,COLOR_BGR2GRAY);
        // trackingCamShift(prev_frame, search_window);

        found_object = CamShift(frame, search_window, criteria);

	    // 3-Bounding rectangle and show the result
	    found_rect = found_object.boundingRect();
	    rectangle(frame, found_rect, Scalar(0,255,0),3);

        imshow("Video Camera",prev_frame);

        // Press Esc to finish
        if(waitKey(1)==27) finish=true;

        prev_frame = frame;
        search_window = found_rect;
    }
    // Release the video camera
    capture.release();
    return 0;
}




int main()
{
    videoCamera();
}
