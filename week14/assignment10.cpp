    #include  <opencv2/opencv.hpp>
    #include <iostream>

    using namespace std;
    using namespace cv;
    double calculatePSNR(Mat original, Mat reconstructed) {
        Mat s1;
        absdiff(original, reconstructed, s1); 
        s1.convertTo(s1, CV_32F);  
        s1 = s1.mul(s1);

        Scalar s = sum(s1);
        double mse = s.val[0] / (double)(original.total());
        if (mse == 0) {
            return numeric_limits<double>::infinity();
        }
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }

    Mat applyDCT(const Mat input) {
        Mat floatInput;
        input.convertTo(floatInput, CV_32F);
        dct(floatInput, floatInput);
        return floatInput;
    }
    Mat applyQuant(Mat input, Mat quantMatrix) {
        Mat floatQuantMatrix;
        quantMatrix.convertTo(floatQuantMatrix, CV_32F);
        Mat output = input.clone();
        
        // Process blocks of 8x8
        for (int i = 0; i < input.rows; i += 8) {
            for (int j = 0; j < input.cols; j += 8) {
                // Get block of 8x8
                Mat block = output(Rect(j, i, 8, 8));
                
                // Quantization
                divide(block, floatQuantMatrix, block);
                
                for (int x = 0; x < block.rows; ++x) {
                    for (int y = 0; y < block.cols; ++y) {
                        block.at<float>(x, y) = round(block.at<float>(x, y));
                    }
                }

                multiply(block, floatQuantMatrix, block);
            }
        }
        return output;
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
        Mat y(512, 512, CV_8UC1);
        image = imread("lena.png", IMREAD_COLOR);
        cvtColor(image, image_Ycbcr, CV_BGR2YCrCb); 

        split(image_Ycbcr, Ycbcr_channels);

        for (int j = 0; j < 512; j++){ 
            for (int i = 0; i < 512; i++) {
                y.at<uchar>(j, i) = 0;
                y.at<uchar>(j, i) = Ycbcr_channels[0].at<uchar>(j,i);
            }
        }

        Mat dct_y = applyDCT(y);

        Mat qm1_dct_quant = applyQuant(dct_y, quantization_mat1);
        Mat qm2_dct_quant = applyQuant(dct_y, quantization_mat2);
        Mat qm3_dct_quant = applyQuant(dct_y, quantization_mat3);

        Mat y_restored_qm1 = applyIDCT(qm1_dct_quant);
        Mat y_restored_qm2 = applyIDCT(qm2_dct_quant);
        Mat y_restored_qm3 = applyIDCT(qm3_dct_quant);


        imshow("Original Y", y);
        imshow("Restored Y QM1", y_restored_qm1);
        imshow("Restored Y QM2", y_restored_qm2);
        imshow("Restored Y QM3", y_restored_qm3);
        double psnr_qm1 = calculatePSNR(y, y_restored_qm1);
        double psnr_qm2 = calculatePSNR(y, y_restored_qm2);
        double psnr_qm3 = calculatePSNR(y, y_restored_qm3);

        cout << "PSNR for QM1: " << psnr_qm1 << " dB" << endl;
        cout << "PSNR for QM2: " << psnr_qm2 << " dB" << endl;
        cout << "PSNR for QM3: " << psnr_qm3 << " dB" << endl;

        waitKey(0);
        return 0;
}