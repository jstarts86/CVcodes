#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat frame;
    Mat canny_left, canny_right;
    VideoCapture cap;
    float rho, theta, a, b, x0, y0, total_rho, total_theta;

    float rho1, theta1, a1, b1, x1, y1, total_rho1, total_theta1;

    Point p1, p2;
    Point p3, p4;

    vector<Vec2f> lines1, lines2;

    Rect rect_left(200, 400, 400, 200);
    Rect rect_right(600, 400, 400, 200);

    if (cap.open("Road.mp4") == 0) {
        cout << "no such file!" << endl;
        waitKey(0);
    }

    int fps = cap.get(CAP_PROP_FPS);
    int time = 0;

    while(1) {
        cap >> frame;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }
        else if (time > 20000) {
            break;
        }
        time = cap.get(CAP_PROP_POS_MSEC);
        canny_left = frame(rect_left);
        canny_right = frame(rect_right);

        cvtColor(canny_left, canny_left, CV_BGR2GRAY);
        blur(canny_left, canny_left, Size(5,5));
        Canny(canny_left, canny_left, 40, 200, 3);
        cvtColor(canny_right, canny_right, CV_BGR2GRAY);
        blur(canny_right, canny_right, Size(5, 5));
        Canny(canny_right, canny_right, 50, 200, 3);

        HoughLines(canny_left, lines1, 1, CV_PI / 180, 60, 0 ,0, CV_PI / 6, CV_PI / 3);
        HoughLines(canny_right, lines2, 1, CV_PI / 180, 70, 0 ,0, CV_PI / 3 * 2, CV_PI / 6 * 5);
        if (lines1.size()) {
            total_rho = 0;
            total_theta = 0;
            for (int i = 0; i < lines1.size(); i++) {
                rho = lines1[i][0];
                theta = lines1[i][1];
                total_rho += rho;
                total_theta += theta;
            }
            total_rho /= lines1.size();
            total_theta /= lines1.size();

            a = cos(total_theta);
            b = sin(total_theta);
            x0 = a * total_rho;
            y0 = b * total_rho;

            p1 = Point(cvRound(x0 + 1000 * (-b)) + 200, cvRound(y0 + 1000 * a) + 400);
            p2 = Point(cvRound(x0 - 1000 * (-b)) + 200, cvRound(y0 - 1000 * a) + 400);
            line(frame, p1, p2, Scalar(0, 0, 255), 3, 8);
        }
        if (lines2.size()) {
            total_rho1 = 0;
            total_theta1 = 0;

            for(int i = 0; i < lines2.size(); i++) {
                rho1 = lines2[i][0];
                theta1 = lines2[i][1];

                total_rho1 += rho1;
                total_theta1 += theta1;
            }
            total_rho1 /= lines2.size();
            total_theta1 /= lines2.size();

            a1 = cos(total_theta1);
            b1 = sin(total_theta1);
            x1 = a1 * total_rho1;
            y1 = b1 * total_rho1;
            p3 = Point(cvRound(x1 + 1000 * (-b1)) + 600, cvRound(y1 + 1000 * a1) + 400);
            p4 = Point(cvRound(x1 - 1000 * (-b1)) + 600, cvRound(y1 -1000 * a1) + 400);
            line(frame, p3, p4, Scalar(0,0, 255), 3, 8);
        }

        namedWindow("Left canny");
        moveWindow("Left canny", 200, 0);
        imshow("Left canny", canny_left);

        namedWindow("Right canny");
        moveWindow("Right canny", 600, 0);
        
        imshow("Right canny", canny_right);

        imshow("Frame", frame);
        waitKey(1000/fps);
    }
    return 0;
}