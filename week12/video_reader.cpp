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
        }
    if (event == EVENT_RBUTTONDOWN)
    {
        mp->in.clear();
        imshow("input", mp->frame);
    }
    }
}


int main() {
    VideoCapture cap1("Timesquare.mp4"); // First video
    VideoCapture cap2("contest.mp4"); // Second video (to be overlayed)

    if (!cap1.isOpened() || !cap2.isOpened()) {
        cerr << "Error opening video stream or file" << endl;
        return -1;
    }

    MouseParams mp;
    mp.out = {Point2f(0, 0), Point2f(cap2.get(CAP_PROP_FRAME_WIDTH), 0), 
              Point2f(cap2.get(CAP_PROP_FRAME_WIDTH), cap2.get(CAP_PROP_FRAME_HEIGHT)),
              Point2f(0, cap2.get(CAP_PROP_FRAME_HEIGHT))};

    namedWindow("Video");
    setMouseCallback("Video", onMouse, &mp);

    Mat frame1, frame2, transformedFrame2;
    while (true) {
        cap1 >> frame1;
        cap2 >> frame2;

        if (frame1.empty() || frame2.empty()) break;

        // Always draw the circles on the frame1
        for (size_t i = 0; i < mp.in.size(); i++) {
            circle(frame1, mp.in[i], 3, Scalar(0, 0, 255), 5);
        }

        if (mp.setPoints && mp.in.size() == 4) {
            Mat homo_mat = getPerspectiveTransform(mp.out, mp.in);
            warpPerspective(frame2, transformedFrame2, homo_mat, frame1.size());

            // Create a mask to overlay transformedFrame2 onto frame1
            Mat mask = Mat::zeros(frame1.size(), CV_8UC1);
            Point points[1][4];
            for (int i = 0; i < 4; i++) {
                points[0][i] = mp.in[i];
            }
            const Point* ppt[1] = { points[0] };
            int npt[] = { 4 };
            fillPoly(mask, ppt, npt, 1, Scalar(255), 8);

            transformedFrame2.copyTo(frame1, mask);
        }

        imshow("Video", frame1);

        char key = (char)waitKey(30);
        if (key == 27) break; // Exit on ESC
    }

    cap1.release();
    cap2.release();
    destroyAllWindows();
    return 0;
}
