#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat applyDCTQuant(Mat input, Mat matrix) {

}


int main(int argc, char* argv[]) {
    Mat image;
    Mat image_Ycbcr;
    Mat quantization_mat1 = (Mat_<double>(8, 8) <<
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99);

    Mat quantization_mat2 = (Mat_<double>(8, 8) <<
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1);
    Mat quantization_mat3 = (Mat_<double>(8, 8) <<
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100,
        100, 100, 100, 100, 100, 100, 100, 100);
    Mat Ycbcr_channels[3]; 
    Mat y(512, 512, CV_8UC1);
    image = imread("lena.png", IMREAD_COLOR);
    cvtColor(image, image_Ycbcr, CV_BGR2YCrCb); 
    split(image_Ycbcr, Ycbcr_channels);
    Mat y = Ycbcr_channels[0].clone();

    Mat qm1 = applyDCTQuant(y, quantization_mat1);
    Mat qm2 = applyDCTQuant(y, quantization_mat2);
    Mat qm3 = applyDCTQuant(y, quantization_mat3);
    
    imshow("Orignal Y", y);
    imshow("QM1", qm1);
    imshow("QM2", qm2);
    imshow("QM3", qm3);
    waitKey(0);
    return 0;
    }