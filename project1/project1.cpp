#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat negative(Mat image);
Mat gamma(Mat image);
Mat histogram(Mat image);
Mat colorSlicing(Mat image);
Mat colorConversion(Mat input);
Mat averageFilter(Mat input);
Mat sharpening(Mat input);
Mat whiteBalancing(Mat input);

Mat negative(Mat image) {
    Mat negative_input = image.clone();
    for(int j = 0; j < image.rows; j++) {
        for(int i = 0; i < image.cols; i++) {
            Vec3b pixel = image.at<Vec3b>(j, i);
            pixel[0] = 255 - pixel[0]; 
            pixel[1] = 255 - pixel[1];
            pixel[2] = 255 - pixel[2]; 
            negative_input.at<Vec3b>(j, i) = pixel;
        }
    }
    return negative_input;
}

Mat gamma(Mat image) {
    Mat gamma_input;
    MatIterator_<uchar> it, end;
    float gamma = 2.5;
    unsigned char pix[256];

    for (int i = 0; i < 256; i++) {
        pix[i] = saturate_cast<uchar>(pow((float)((i)/ 255.0), gamma) * 255.0f); // it would be too large if you didnt divide by 255
    }
    gamma_input = image.clone();

    for (int j = 0; j < image.rows; j++) {
        for (int i = 0; i < image.cols; i++) {
            Vec3b& pixel = gamma_input.at<Vec3b>(j, i);
            pixel[0] = pix[pixel[0]]; 
            pixel[1] = pix[pixel[1]]; 
            pixel[2] = pix[pixel[2]]; 
        }
    }
    return gamma_input;
}


Mat histogram(Mat image){
    Mat HSV, intensity_change;
    vector<Mat> ic(3);
    int rows = image.rows; 
    int cols = image.cols; 
    uchar* h;
    uchar* s;
    uchar* v;
    cvtColor(image, HSV, COLOR_BGR2HSV); 
    split(HSV, ic);
    equalizeHist(ic[2], ic[2]);
    merge(ic, intensity_change);
    cvtColor(intensity_change, intensity_change, COLOR_HSV2BGR);
    return intensity_change;
}
Mat colorSlicing(Mat image){
    Mat HSV, mask_out; 
    vector<Mat> ic(3);
    vector<Mat> mo(3);
    vector<Mat> cc(3);
    int rows = image.rows; 
    int cols = image.cols; 
    uchar* h;
    uchar* s;
    uchar* v;
    cvtColor(image, HSV, COLOR_BGR2HSV); 
    split(HSV, ic);
    split(HSV, mo);
    split(HSV, cc);
    equalizeHist(ic[2], ic[2]);
    for (int j = 0; j < rows; j++) {
        h = mo[0].ptr<uchar>(j); 
        s = mo[1].ptr<uchar>(j);
        for (int i = 0; i < cols; i++) {
            if (h[i] > 9 && h[i] < 23) s[i] = s[i];
            else s[i] = 0; 
        }
    }
    for (int j = 0; j < rows; j++) {
        h = cc[0].ptr<uchar>(j);
        s = cc[1].ptr<uchar>(j);
        for (int i = 0; i < cols; i++) {
            if (h[i] + 50 > 179) h[i] = h[i] + 50 - 179; 
            else h[i] += 50;
        }    
    }   
    merge(mo, mask_out);
    cvtColor(mask_out, mask_out, COLOR_HSV2BGR); 
    return mask_out;
}
Mat colorConversion(Mat image){
    Mat HSV, change_color;
    vector<Mat> cc(3);
    int rows = image.rows; 
    int cols = image.cols; 
    uchar* h;
    uchar* s;
    uchar* v;
    cvtColor(image, HSV, COLOR_BGR2HSV); 
    split(HSV, cc);
    for (int j = 0; j < rows; j++) {
        h = cc[0].ptr<uchar>(j);
        s = cc[1].ptr<uchar>(j);
        for (int i = 0; i < cols; i++) {
            if (h[i] + 50 > 179) h[i] = h[i] + 50 - 179; 
            else h[i] += 50;
        }    
    }
    merge(cc, change_color);
    cvtColor(change_color, change_color, COLOR_HSV2BGR);
    return change_color;
}
Mat averageFilter(Mat image){
    Mat avg;
    blur(image, avg, Size(9, 9));
    return avg;
}
Mat sharpening(Mat input){
    Mat blurred, mask, sharpened;
    blur(input, blurred, Size(9, 9));
    subtract(input, blurred, mask);
    add(input, mask, sharpened);
    return sharpened;
}
Mat whiteBalancing(Mat input){
    Mat bgr_channels[3]; 
    split(input, bgr_channels);
    double avg;
    int sum,temp,i, j, c;
    for (c = 0; c < input.channels(); c++) { 
        sum = 0;
        avg = 0.0f;
        for (i = 0; i < input.rows; i++) {
            for (j = 0; j < input.cols; j++) {
                sum += bgr_channels[c].at<uchar>(i, j); }
        }
        avg = sum / (input.rows * input.cols); for (i = 0; i < input.rows; i++) {
            for (j = 0; j < input.cols; j++) {
                temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j); 
                if (temp>255) bgr_channels[c].at<uchar>(i, j) = 255;
                else bgr_channels[c].at<uchar>(i, j) = temp;
            }
        } 
    }
    merge(bgr_channels, 3, input);
    return input;
}

int main() {
    int currentFrame = 0, startFrame = 0;
    Mat input, originalFrame;
    int fps;
    int delay;
    VideoCapture cap;
    if (!cap.open("video.mp4")) {
        cout << "Failed to open the video file!" << endl;
        waitKey(0);
        return -1; // Exit the program
    }
    char choice = 'r'; 
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
            imshow("video", input);
            int keyValue = waitKey(delay);
            char key = (char) keyValue;
            if (key == 27) { 
                break;
            } else if (key >= 0) {
            choice = key;
        }
        }
        
        currentFrame++;
    }
    return 0;
}