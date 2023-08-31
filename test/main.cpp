#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    Mat image;
    image = imread("Lena.png", IMREAD_COLOR);
    if (image.empty()) {
        cout << "COuld not open or find the image" << endl;
        return -1;
    }

    namedWindow("Lena", WINDOW_AUTOSIZE);
    imshow("Lena", image);

    waitKey(0);
}