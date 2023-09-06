#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image = imread("lena.png", 0);
    Mat negative_img = image.clone();
    for( int j = 0;  j < image.rows; j++) {
        for (int i =0; i < image.cols; i++) {
            negative_img.at<uchar>(j,i) = 255 - image.at<uchar>(j, i);// whenever you access a pixel then you minus it from 255
        }
    }   
    imshow("Input image", image);
    imshow("Negative transformation", negative_img);

    waitKey(0);
    return 0;
}