#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat half_unsharp(Mat input);

Mat half_median(Mat input);



Mat half_unsharp(Mat input) {
    Mat blurred_right, mask_right, sharpened_right;
    Mat input_filtered = input.clone();
    Rect rect(input.cols/2, 0, input.cols/2, input.rows);
    Mat input_right = input(rect);
    Mat input_filtered_right_half = input_filtered(rect);
    GaussianBlur(input_filtered_right_half,blurred_right, Size(3,3), 0, 0, BORDER_DEFAULT);
    subtract(input_right, blurred_right, mask_right);
    add(input_right, mask_right, sharpened_right);
    sharpened_right.copyTo(input_filtered(rect));
    return input_filtered;
}

Mat half_median(Mat input) {
    Mat input_blurred = input.clone();
    Rect rect(0,0,input.cols/2, input.rows);
    Mat input_blurred_right = input_blurred(rect);
    Mat input_right = input(rect);
    medianBlur(input_right, input_blurred_right, 9);
    input_blurred_right.copyTo(input_blurred(rect));
    return input_blurred;
}
int main() {
    Mat moon = imread("moon.jpeg", 0);
    Mat moon_filtered = half_unsharp(moon);

    Mat saltnpepper = imread("saltnpepper.png", 0);
    Mat saltnpepper_filtered = half_median(saltnpepper);



    imshow("moon", moon);
    imshow("moon_filtered", moon_filtered);
    imshow("saltnpepper", saltnpepper);
    imshow("saltnpepper_filtered", saltnpepper_filtered);

    waitKey(0);
    return 0;
}
