#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    Mat input = imread("lena.png", 0);
    Mat result = input.clone();
    int value;
    float gamma = 10;
    unsigned char pix[256];
    for (int j = 0 ; j < input.rows ; j++) {
        for (int i = 0; i < input.cols; i++) {
            result.at<uchar>(input.rows - j- 1, i) = input.at<uchar>(j, i);
        }
    }
    imshow("gray image", input);
    imshow("result", result);

    waitKey(0);
    return 0;
}