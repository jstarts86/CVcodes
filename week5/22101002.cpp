#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;




int main() {
    Mat finger_result, adaptive1_result, adapt_result;
    Mat finger_print = imread("finger_print.png");
    Mat adaptive1 = imread("adaptive_1.jpg");
    Mat adapt = imread("adaptive.png");

    cvtColor(finger_print,finger_print, CV_BGR2GRAY);
    cvtColor(adaptive1, adaptive1, CV_BGR2GRAY);
    cvtColor(adapt,adapt, CV_BGR2GRAY);

    threshold(finger_print,finger_result,0, 255, THRESH_BINARY | THRESH_OTSU);
    adaptiveThreshold(adaptive1, adaptive1_result, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);
    adaptiveThreshold(adapt, adapt_result, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);










   // threshold(finger_print, binary_fingerprint,0,255, THRESH_BINARY);


    imshow("finger_print", finger_result);
    imshow("adaptive_1", adaptive1_result);
    imshow("adaptive", adapt_result);

    waitKey(0);
    return 0;
}