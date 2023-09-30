#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat canny_edges_left (Mat input);
Mat canny_edges_right (Mat input);

Mat line_merge(Mat input);


Mat canny_edges_left(Mat input){
    Rect left_rect(200,400,400,200);
    //dsiplay canny edges
    Mat input_gray = input.clone();
    // Make a gray_scale image
    cvtColor(input, input_gray, CV_BGR2GRAY, 0);
    // Blur a gray image
    Mat input_gray_blur;
    blur(input_gray,input_gray_blur,Size(5,5));
    // Canny edge
    Mat result;
    Canny(input,result, 10,60,3);
    //split into two windows
    Mat result_left = result(left_rect);
    return result_left;
}
Mat canny_edges_right(Mat input){
    Rect right_rect(600,400,400,200);
    //dsiplay canny edges
    Mat input_gray = input.clone();
    // Make a gray_scale image
    cvtColor(input, input_gray, CV_BGR2GRAY, 0);
    // Blur a gray image
    Mat input_gray_blur;
    blur(input_gray,input_gray_blur,Size(5,5));
    // Canny edge
    Mat result;
    Canny(input,result, 10,60,3);
    //split into two windows
    Mat result_right = result(right_rect);
    return result_right;
}

Mat line_merge_left(Mat input) {
    Mat edge, result;
    Rect right_rect(220,400,400,200); 
    Mat input_right = input(right_rect);
    float rho, theta, a, b, x0,y0;
    Point p1, p2;
    vector<Vec2f> lines;
    result = input.clone();
    // gray image
    Mat gray_image;
    cvtColor(input_right, gray_image, CV_BGR2GRAY);
    // blur gray image
    Mat input_gray_blur;
    blur(gray_image, input_gray_blur,Size(5,5));
    // Canny Edge
    Canny(input_gray_blur, edge,10,60);
    HoughLines(edge, lines,1 ,CV_PI / 180, 150,0, CV_PI, (30 * (CV_PI/180)),(60 * (CV_PI/180)));
    float avg_rho = 0;
    float avg_theta = 0;
    int count = 0;
    for(int i = 0; i < lines.size(); i++) {
        rho = lines[i][0];
        theta = lines[i][1];
        avg_rho += rho;
        avg_theta += theta;
        count++;
    }
    avg_rho = avg_rho/count;
    avg_theta = avg_theta/count;
    a = cos(avg_theta);
    b = sin(avg_theta);

    x0 = a * avg_rho;
    y0 = b * avg_rho;

    p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
    p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));

    line(result,p1,p2,Scalar(0,0,255),3,8);
    return result;
}


Mat line_merge_right(Mat input) {
    Mat edge, result;
    Rect right_rect(600,400,400,200); 
    Mat input_right = input(right_rect);
    float rho, theta, a, b, x0,y0;
    Point p1, p2;
    vector<Vec2f> lines;
    result = input.clone();
    // gray image
    Mat gray_image;
    cvtColor(input_right, gray_image, CV_BGR2GRAY);
    // blur gray image
    Mat input_gray_blur;
    blur(gray_image, input_gray_blur,Size(5,5));
    // Canny Edge
    Canny(input_gray_blur, edge,10,60);
    HoughLines(edge, lines,1 ,CV_PI / 180, 150,0, CV_PI, (120 * (CV_PI/180)),(150 * (CV_PI/180)));
    float avg_rho = 0;
    float avg_theta = 0;
    int count = 0;
    for(int i = 0; i < lines.size(); i++) {
        rho = lines[i][0];
        theta = lines[i][1];
        avg_rho += rho;
        avg_theta += theta;
        count++;
    }
    avg_rho = avg_rho/count;
    avg_theta = avg_theta/count;
    a = cos(avg_theta);
    b = sin(avg_theta);

    x0 = a * avg_rho;
    y0 = b * avg_rho;

    p1 = Point(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
    p2 = Point(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));

    line(result,p1,p2,Scalar(0,0,255),3,8);
    return result;
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
    
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000/fps;
    int endFrame = fps * 20;

    while(1) {
        cap >> input;
        if(input.empty() || currentFrame > endFrame) {
            break;
        }
        if (currentFrame >= startFrame) {
            Mat canny_frame_left = canny_edges_left(input);
            Mat canny_frame_right = canny_edges_right(input);
            Mat hough_left = line_merge_left(input);
            Mat hough_right = line_merge_right(hough_left);
            namedWindow("Left canny");
            namedWindow("Right canny");
            moveWindow("Left canny", 200, 0);
            moveWindow("Right canny", 600, 0);
            imshow("Image", hough_right);
            imshow("Left canny", canny_frame_left);
            imshow("Right canny", canny_frame_right);
            waitKey(delay);
        }
        currentFrame++;
    }
    return 0;
}
