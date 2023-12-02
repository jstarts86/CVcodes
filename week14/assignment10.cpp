#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
Mat applyDCT(Mat input) {
    Mat floatInput;
    input.convertTo(floatInput, CV_32F);
    dct(floatInput, floatInput);
    return floatInput;
}

Mat applyQuantization(Mat input, Mat quantMatrix) {
    Mat quantizedInput = input.clone();
    for (int i = 0; i < input.rows; i += 8) {
        for (int j = 0; j < input.cols; j += 8) {
            Rect roi(j, i, 8, 8);
            Mat block = quantizedInput(roi);
            block = block / quantMatrix;
            block.copyTo(quantizedInput(roi));
        }
    }
    return quantizedInput;
}

Mat applyIDCT(Mat input) {
    Mat output;
    idct(input, output);
    output.convertTo(output, CV_8U);
    return output;
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

    image = imread("lena.png", IMREAD_COLOR);
    cvtColor(image, image_Ycbcr, CV_BGR2YCrCb); 

    split(image_Ycbcr, Ycbcr_channels);

    Mat y = Ycbcr_channels[0].clone();
    Mat dct_y = applyDCT
    Mat qm1, qm2, qm3;

    Mat qm1_dct_quantized = applyQuantization(dct_y, quantization_mat1);
    Mat qm2_dct_quantized = applyQuantization(dct_y, quantization_mat2);
    Mat qm3_dct_quantized = applyQuantization(dct_y, quantization_mat3);

    Mat y_restored_qm1 = applyIDCT(qm1_dct_quantized);
    Mat y_restored_qm2 = applyIDCT(qm2_dct_quantized);
    Mat y_restored_qm3 = applyIDCT(qm3_dct_quantized);

    imshow("Original Y", Ycbcr_channels[0]);
    imshow("Restored Y QM1", y_restored_qm1);
    imshow("Restored Y QM2", y_restored_qm2);
    imshow("Restored Y QM3", y_restored_qm3);



    

    imshow("Orignal Y", y);
    imshow("QM1", qm1);
    imshow("QM2", qm2);
    imshow("QM3", qm3);
    waitKey(0);
    return 0;
    }