#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image = imread("lena.png",0);
    Mat f_img, log_img;
    double c = 1.5f; //scale constant brightens the image after log transfromation
    image.convertTo(f_img, CV_32F); // need to convert type of image to floating point
    f_img = abs(f_img) + 1; // log 0 most be avoided
    log(f_img, f_img);
    normalize(f_img, f_img, 0 , 255, NORM_MINMAX); //normalize image to (0-255) if you take log transformation, most will be very low so it would be too dark
    convertScaleAbs(f_img, log_img, c); // scaling by c, conversion to an unsigned 8-bit type
    imshow("Input image", image);
    imshow("Log transformation", log_img);

    waitKey(0);
    
}