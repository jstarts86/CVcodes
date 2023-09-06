#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image = imread("lena.png", 0);
    Mat gamma_img;
    MatIterator_<uchar> it, end;
    float gamma = 0.5;
    unsigned char pix[256];

    for (int i = 0; i < 256; i++) {
        pix[i] = saturate_cast<uchar>(pow((float)((i)/ 255.0), gamma) * 255.0f); // it would be too large if you didnt divide by 255
    }
    gamma_img = image.clone();

    for(int j = 0; j < image.rows; j++) {
        for (int i = 0; i < image.cols; i++){
            gamma_img.at<uchar> (j, i) = pix[gamma_img.at<uchar>(j,i)];
        }
    }
    imshow("input image", image);
    imshow("gamma transformation", gamma_img);
}