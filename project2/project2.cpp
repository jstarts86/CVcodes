#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    Mat frame;
    Mat canny_left, canny_right, canny_center;
    VideoCapture cap;
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

    //Rect rect_left(100, 280, 260, height); // Adjusted Y coordinate
    //Rect rect_right(360, 280, 200, height); // Adjusted X and Y coordinatesç

    // Rect rect_left(150, 200, 260, 200);
    // Rect rect_right(360, 200, 200, 200);
    Rect rect_center(330, 200, 70, 240);
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

        // // Print frame dimensions
        // cout << "Frame dimensions: " << frame.cols << "x" << frame.rows << endl;

        // // Check if ROIs are within the frame
        // if(rect_left.x + rect_left.width > frame.cols || rect_left.y + rect_left.height > frame.rows ||
        // rect_right.x + rect_right.width > frame.cols || rect_right.y + rect_right.height > frame.rows) {
        //     // Handle error
        //     cout << "ROI is out of frame bounds" << endl;
        //     break;
        // }
        // canny_left = frame(rect_left);
        // canny_right = frame(rect_right);
        canny_center = frame(rect_center);
        // Update the function names according to your OpenCV version
        //left roi
        // cvtColor(canny_left, canny_left, COLOR_BGR2GRAY);
        // blur(canny_left, canny_left, Size(5,5));
        // Canny(canny_left, canny_left, 70, 200, 3);
        
        
        //right roi
        // cvtColor(canny_right, canny_right, CV_BGR2GRAY);
        // blur(canny_right, canny_right, Size(3, 3));
        // Canny(canny_right, canny_right, 70, 200, 3);


        //center roi 
        cvtColor(canny_center, canny_center, CV_BGR2GRAY);
        blur(canny_center, canny_center, Size(3, 3));
        Canny(canny_center, canny_center, 30, 90, 3);

        // HoughLines(canny_left, lines1, 1, CV_PI / 180, 60, 0 ,0, CV_PI / 6, CV_PI / 3);
        // HoughLines(canny_right, lines2, 1, CV_PI / 180, 70, 0 ,0, CV_PI / 3 * 2, CV_PI / 6 * 5);
        HoughLines(canny_center, lines3, 1, CV_PI / 180, 70, 0 ,0, CV_PI / 18 * 8, CV_PI / 5 * 9);


        // if (lines1.size()) {
        //     total_rho = 0;
        //     total_theta = 0;
        //     for (int i = 0; i < lines1.size(); i++) {
        //         rho = lines1[i][0];
        //         theta = lines1[i][1];
        //         total_rho += rho;
        //         total_theta += theta;
        //     }
        //     total_rho /= lines1.size();
        //     total_theta /= lines1.size();

        //     a = cos(total_theta);
        //     b = sin(total_theta);
        //     x0 = a * total_rho;
        //     y0 = b * total_rho;

        //     p1 = Point(cvRound(x0 + 1000 * (-b)) + rect_left.x, cvRound(y0 + 1000 * a) + rect_left.y);
        //     p2 = Point(cvRound(x0 - 1000 * (-b)) + rect_left.x, cvRound(y0 - 1000 * a) + rect_left.y);
        //     line(frame, p1, p2, Scalar(0, 0, 255), 3, 8);
        // }
        // if (lines2.size()) {
        //     total_rho1 = 0;
        //     total_theta1 = 0;

        //     for(int i = 0; i < lines2.size(); i++) {
        //         rho1 = lines2[i][0];
        //         theta1 = lines2[i][1];

        //         total_rho1 += rho1;
        //         total_theta1 += theta1;
        //     }
        //     total_rho1 /= lines2.size();
        //     total_theta1 /= lines2.size();

        //     a1 = cos(total_theta1);
        //     b1 = sin(total_theta1);
        //     x1 = a1 * total_rho1;
        //     y1 = b1 * total_rho1;
        //     p3 = Point(cvRound(x1 + 1000 * (-b1)) + rect_right.x, cvRound(y1 + 1000 * a1) + rect_right.y);
        //     p4 = Point(cvRound(x1 - 1000 * (-b1)) + rect_right.x, cvRound(y1 -1000 * a1) + rect_right.y);
        //     line(frame, p3, p4, Scalar(0,0, 255), 3, 8);
        // }
        if (lines3.size()) {
            putText(frame, "Lane Departure", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
            total_rho2 = 0;
            total_theta2 = 0;
            for(int i = 0; i < lines3.size(); i++) {
                rho2 = lines3[i][0];
                theta2 = lines3[i][1];
                total_rho2 += rho2;
                total_theta2 += theta2;
            }
            total_rho2 /= lines3.size();
            total_theta2 /= lines3.size();
            a2 = cos(total_theta2);
            b2 = sin(total_theta2);
            x2 = a2 * total_rho2;
            y2 = b2 * total_rho2;
            p5 = Point(cvRound(x2 + 1000 * (-b2)) + rect_center.x, cvRound(y2 + 1000 * a2) + rect_center.y);
            p6 = Point(cvRound(x2 - 1000 * (-b2)) + rect_center.x, cvRound(y2 -1000 * a2) + rect_center.y);
            line(frame, p5, p6, Scalar(0,0, 255), 3, 8);
        }
        // if (lines3.size()) {
        //     putText(frame, "Lane Departure", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
        
        // }
        imshow("center canny", canny_center);
        // namedWindow("Left canny");
        // imshow("Left canny", canny_left);

        // namedWindow("Right canny");
        
        // imshow("Right canny", canny_right);

        imshow("Frame", frame);
        waitKey(250/fps);
    }
    return 0;
}