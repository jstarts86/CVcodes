#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    Mat frame;
    Mat canny_left, canny_right, canny_center, car;
    VideoCapture cap;
    Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2(); 
    Mat image, foregroundMask, backgroundImg, foregroundImg;
    float rho, theta, a, b, x0, y0, total_rho, total_theta;

    float rho1, theta1, a1, b1, x1, y1, total_rho1, total_theta1;

    float rho2, theta2, a2, b2, x2, y2, total_rho2, total_theta2;

    Point p1, p2;
    Point p3, p4;
    Point p5, p6;


    vector<Vec2f> lines1, lines2, lines3;

    // Assuming you want to keep the width and height of the ROIs the same
    int width = 400;
    int height = 200;



    // Rect rect_left(150, 200, 260, 200);
    // Rect rect_right(360, 200, 200, 200);
    Rect rect_center(330, 200, 70, 230);
    Rect roi(280, 230, 210, 100);
    
    if (cap.open("Project2_video.mp4") == 0) {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    Point text = Point(20,20);
    int fps = cap.get(CAP_PROP_FPS);
    int time = 0;
    int consecutive = 0;
    Mat prevForegroundMask;
    bool isWhite = false; 
    while(1) {
        cap >> frame;
        
        if (frame.empty()) {
            cout << "Empty frame or end of video" << endl;
            break;
        }

        canny_center = frame(rect_center);


        // //center roi 
        cvtColor(canny_center, canny_center, CV_BGR2GRAY);
        blur(canny_center, canny_center, Size(3, 3));
        Canny(canny_center, canny_center, 10, 60, 3);

        HoughLines(canny_center, lines3, 1, CV_PI / 180, 80, 0 ,0, (CV_PI * 5) / 6, (CV_PI * 10) /9 );

        if (lines3.size() > 0) {
            consecutive++;
        } else {
            consecutive = 0;
        }
        if (consecutive > 2) {
            putText(frame, "Lane Departure", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
            // total_rho2 = 0;
            // total_theta2 = 0;
            // for(int i = 0; i < lines3.size(); i++) {
            //     rho2 = lines3[i][0];
            //     theta2 = lines3[i][1];
            //     total_rho2 += rho2;
            //     total_theta2 += theta2;
            // }
            // total_rho2 /= lines3.size();
            // total_theta2 /= lines3.size();
            // a2 = cos(total_theta2);
            // b2 = sin(total_theta2);
            // x2 = a2 * total_rho2;
            // y2 = b2 * total_rho2;
            // p5 = Point(cvRound(x2 + 1000 * (-b2)) + rect_center.x, cvRound(y2 + 1000 * a2) + rect_center.y);
            // p6 = Point(cvRound(x2 - 1000 * (-b2)) + rect_center.x, cvRound(y2 -1000 * a2) + rect_center.y);
            // line(frame, p5, p6, Scalar(0,0, 255), 3, 8);
        }
        

        // if (lines3.size()) {
        //     putText(frame, "Lane Departure", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
        //
        // }

        
        bg_model->apply(frame, foregroundMask);
        GaussianBlur(foregroundMask, foregroundMask, Size(11, 11), 3.5, 3.5);
        threshold(foregroundMask, foregroundMask, 10, 255, THRESH_BINARY); 
        image = foregroundMask(roi);

        if (!prevForegroundMask.empty()) {
            Mat frameDiff;
            absdiff(foregroundMask, prevForegroundMask, frameDiff); 

            int thresholdValue = 30; 
            int changedPixels = countNonZero(frameDiff > thresholdValue); 

            double totalPixels = frameDiff.total();
            double changedPercentage = (double(changedPixels) / totalPixels) * 100.0; 

            if (changedPercentage > 30.0) {
                int whitePixels = countNonZero(foregroundMask);
                int blackPixels = totalPixels - whitePixels;

                isWhite = whitePixels > blackPixels; 
                cout << "The frame is mostly " << (isWhite ? "white." : "black.") << endl;
            }
        }
        if (!isWhite) {
            int whitePixels = countNonZero(image);
            double totalPixels = image.total();
            double whitePercentage = (double(whitePixels) / totalPixels) * 100.0;

    // Check if white pixels are more than 90%
            if (whitePercentage > 90.0) {
                putText(frame, "Front Car is Moving", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
                cout << "LEAVE " << endl;
            }
        }
        if (isWhite) {
            int whitePixels = countNonZero(image);
            double totalPixels = image.total();
            double whitePercentage = (double(whitePixels) / totalPixels) * 100.0;

    // Check if white pixels are more than 90%
            if (whitePercentage <= 30.0) {
                putText(frame, "Front Car is Moving", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
                cout << "LEAVE " << endl;
            }
        }

        prevForegroundMask = foregroundMask.clone(); 
        // imshow("foreground mask", foregroundMask); 

       // imshow("center canny", canny_center);
        imshow("Frame", frame);
        waitKey(1);
    }
    return 0;
}
