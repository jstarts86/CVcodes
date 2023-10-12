#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat negative(Mat image);
Mat gamma(Mat input);
Mat histogram(Mat input);
Mat colorSlicing(Mat input);
Mat colorConversion(Mat input);
Mat averageFilter(Mat input);
Mat sharpening(Mat input);
Mat whiteBalancing(Mat input);

Mat negative(Mat image) {
    Mat negative_img = image.clone();
    for( int j = 0;  j < image.rows; j++) {
        for (int i =0; i < image.cols; i++) {
            negative_img.at<uchar>(j,i) = 255 - image.at<uchar>(j, i);// whenever you access a pixel then you minus it from 255
        }
    }
    return negative_img;   
}
int main() {
    int currentFrame = 0, startFrame = 0;
    Mat input;
    int fps;
    int delay;
    VideoCapture cap;
    if (cap.open("Road.mp4") == 0 ){
        cout << "no such file!" << endl;
        waitKey(0);
    }
    int choice;
    cin >> choice;
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000/fps;
    int endFrame = fps * 20;

    while(1) {
        cap >> input;
        if(input.empty()) {
            break;
        }
        if (currentFrame >= startFrame) {
            // Apply the chosen operation
            switch(choice) {
                case 'n': input = negative(input); break;
                case 'g': input = gamma(input); break;
                case 'h': input = histogram(input); break;
                case 's': input = colorSlicing(input); break;
                case 'c': input = colorConversion(input); break;
                case 'a': input = averageFilter(input); break;
                case 'u': input = sharpening(input); break;
                case 'w': input = whiteBalancing(input); break;
                case 'r': break;
                default: break;
            }
            imshow("Processed Video", input);
            waitKey(delay);
        }
        currentFrame++;
    }
    return 0;
}