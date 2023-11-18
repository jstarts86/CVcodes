#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>

using namespace cv;
using namespace std;

struct MouseParams {
    Mat frame;
    vector<Point2f> in, out;
    bool setPoints = false;
};

void onMouse(int event, int x, int y, int, void* param) {
    MouseParams* mp = (MouseParams*)param;
    if (event == EVENT_LBUTTONDOWN) {
        if (mp->in.size() == 4) {
            mp->setPoints = true;
        }
        else {
            mp->in.push_back(Point2f(x, y));
            Mat frameWithCircle = mp->frame.clone();
            for (size_t i  = 0; i < mp->in.size(); i++)
            {
                circle(mp->frame, mp->in[i], 3, Scalar(0, 0, 255), 5);
            }
            imshow("Video with Circles", mp->frame);
        }
    if (event == EVENT_RBUTTONDOWN)
    {
        mp->in.clear();
        imshow("input", mp->frame);
    }
    }
}


int main() {
    VideoCapture cap("Timesquare.mp4"); // Replace with your video file

    if (!cap.isOpened()) {
        cerr << "Error opening video stream or file" << endl;
        return -1;
    }

    MouseParams mp;
    mp.out.push_back(Point2f(0, 0));
    mp.out.push_back(Point2f(300, 0));
    mp.out.push_back(Point2f(300, 300));
    mp.out.push_back(Point2f(0, 300));

    namedWindow("Video");
    namedWindow("Video with Circles");
    setMouseCallback("Video", onMouse, &mp);

    while (true) {
        cap >> mp.frame;
        if (mp.frame.empty()) break;

        imshow("Video", mp.frame);

        if (mp.setPoints && mp.in.size() == 4) {
            Mat result;
            Mat homo_mat = getPerspectiveTransform(mp.in, mp.out);
            warpPerspective(mp.frame, result, homo_mat, Size(300, 300));
            imshow("Transformed", result);
        }

        char key = (char)waitKey(30);
        if (key == 27) break; // Exit on ESC
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
