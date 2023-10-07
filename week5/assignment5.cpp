#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;




int main() {
    Mat finger_result, bgdModel, fgdModel, finger_background, finger_foreground;
    Mat finger_print = imread("finger_print.png", 0);
    Mat adaptive1 = imread("adaptive_1.jpg", 0);
    Mat adapt = imread("adaptive.png", 0);

    // cvtColor(finger_print,finger_print, CV_BGR2GRAY);
    // cvtColor(adaptive1, adaptive1, CV_BGR2GRAY);
    // cvtColor(adapt,adapt, CV_BGR2GRAY);


    // Grab cut on finger_print.png
    Rect rectangle(15,0,155,240);
    grabCut(finger_print, finger_result, rectangle, bgdModel, fgdModel, 10 , GC_INIT_WITH_RECT);
    compare(finger_result, GC_PR_FGD, finger_result, CMP_EQ);
    finger_foreground = Mat(finger_print.size(), CV_8UC3, Scalar(255,255,255));
    finger_print.copyTo(finger_foreground, finger_result);
    imshow("sdad", finger_result);











   // threshold(finger_print, binary_fingerprint,0,255, THRESH_BINARY);


    imshow("finger_print", finger_print);
    imshow("adaptive_1", adaptive1);
    imshow("adaptive", adapt);

    waitKey(0);
    return 0;
}