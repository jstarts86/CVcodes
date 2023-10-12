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
    for(int j = 0; j < image.rows; j++) {
        for(int i = 0; i < image.cols; i++) {
            Vec3b pixel = image.at<Vec3b>(j, i);
            pixel[0] = 255 - pixel[0]; // Blue channel
            pixel[1] = 255 - pixel[1]; // Green channel
            pixel[2] = 255 - pixel[2]; // Red channel
            negative_img.at<Vec3b>(j, i) = pixel;
        }
    }
    return negative_img;
}

Mat gamma(Mat input) {
    return input;
}
Mat histogram(Mat input){
    return input;
}
Mat colorSlicing(Mat input){
    return input;
}
Mat colorConversion(Mat input){
    return input;
}
Mat averageFilter(Mat input){
    return input;
}
Mat sharpening(Mat input){
    return input;
}
Mat whiteBalancing(Mat input){
    return input;
}
int main() {
    int currentFrame = 0, startFrame = 0;
    Mat input, originalFrame;
    int fps;
    int delay;
    VideoCapture cap;
    if (!cap.open("Road.mp4")) {
        cout << "Failed to open the video file!" << endl;
        waitKey(0);
        return -1; // Exit the program
    }
    char choice = 'r'; 
    cin >> choice;
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000/fps;
    while(1) {
        cap >> input;
        if(input.empty()) {
            break;
        }
        originalFrame = input.clone();
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
                case 'r': input = originalFrame; break; // Reset to original frame
                default: break; 
            }
            imshow("Processed Video", input);
            waitKey(delay);
        }
        currentFrame++;
    }
    return 0;
}