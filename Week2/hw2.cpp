#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    Mat input = imread("lena.png", 0);
    Mat result = Mat(input.rows, input.cols, CV_8U);
    int value;
    float gamma = 10;
    unsigned char pix[256];
    for(int i = 0; i < 256; i++){
        pix[i] = saturate_cast<uchar>(pow((float)((i)/255.0), gamma) * 255.0f);
    }
    for (int j = 0 ; j < input.rows ; j++) {
        for (int i = 0; i < input.cols; i++) {
            value = input.at<uchar>(j, i);
            if (value < 127) {
                result.at<uchar>(input.rows- i-1, j) = 255 - input.at<uchar>(j, i);
            } else {
                result.at<uchar> (input.rows- i- 1, j) = pix[input.at<uchar>(j,i)];
            }
        }
    }
    imshow("gray image", input);
    imshow("result", result);

    waitKey(0);
    return 0;
}