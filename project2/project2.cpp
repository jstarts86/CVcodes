#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    Mat frame;
    Mat canny_left, canny_right, canny_center;
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
    Mat image, foregroundMask, backgroundImg, foregroundImg;
    VideoCapture cap;

    Point p1, p2;
    Point p3, p4;
    Point p5, p6;


    vector<Vec2f> lines1, lines2, lines3;
    //Rect rect_car(330,200, 120, 240);
    // Rect rect_left(150, 200, 260, 200);
    // Rect rect_right(360, 200, 200, 200);
    Rect rect_center(330, 200, 70, 250);
    //Rect roi(200, 200, 300, 280);
    if (cap.open("Project2_video.mp4") == 0) {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    Point text = Point(20,20);
    int fps = cap.get(CAP_PROP_FPS);
    int time = 0;

    while(1) {
        cap >> frame;
        if (frame.empty()) {
            // Handle the empty frame, e.g., break the loop
            cout << "Empty frame or end of video" << endl;
            break;
        }


        canny_center = frame(rect_center);
        //center roi 
        cvtColor(canny_center, canny_center, CV_BGR2GRAY);
        blur(canny_center, canny_center, Size(3, 3));
        Canny(canny_center, canny_center, 10, 60, 3);
        
        HoughLines(canny_center, lines3, 1, CV_PI / 180, 70, 0 ,0, CV_PI / 18, CV_PI / 5 * 9);

        if (foregroundMask.empty()) {
            foregroundMask.create(frame.size(), frame.type());
        }
            
            
        bg_model->apply(frame, foregroundMask);
        GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);
        threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY); 
        //foregroundImg = Scalar::all(0);

        if (!backgroundImg.empty()) {
            imshow("mean background image", backgroundImg);
        }

        if (lines3.size()) {
            putText(frame, "Lane Departure", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
        }

        imshow("center canny", canny_center);
        imshow("Left canny", canny_left);

        // namedWindow("Right canny");
        
        // imshow("Right canny", canny_right);
        //frame.copyTo(foregroundImg, foregroundMask);
        // backgroundImg: The output background image. 
        // bg_model->getBackgroundImage(backgroundImg);
        // imshow("foreground mask", foregroundMask); 
        // imshow("foreground image", foregroundImg);
        imshow("Frame", frame);
        waitKey(250/fps);
    }
    return 0;
}