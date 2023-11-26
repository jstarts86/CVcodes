#include  <opencv2/opencv.hpp>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>


using namespace std;
using namespace cv;

int main() {
    VideoCapture cap = VideoCapture(0);
    int successes = 0;

    int numBoards = 5;
    int numCornersHor = 10;
    int numCornersVer = 7;
    int Rect_size = 20;

    int numSquares = (numCornersHor -1) * (numCornersVer - 1);
    Size board_sz = Size(numCornersHor, numCornersVer);

    vector<vector<Point3f> > object_points;
    vector<vector<Point2f> > image_points;
    vector<Point2f> corners;
    vector<Point3f> obj;


    for(int i = 0; i < numCornersHor; i++) {
        for (int j = 0; j < numCornersVer; j++) {
            obj.push_back(Point3f(i *Rect_size,j * Rect_size, 0.0f));
        }
    }
    Mat img;
    Mat gray;
    cap >> img;
    cout << "Image size:" << img.size() << endl;

    while (successes < numBoards) {
        cap >> img;
        cvtColor(img, gray, CV_RGB2GRAY);
        if (img.empty()) break;
        if (waitKey(1) == 27) break;

        bool found = findChessboardCorners( gray, board_sz, corners, CALIB_CB_ADAPTIVE_THRESH);

        if (found == 1) {
            cornerSubPix(gray, corners, Size(11,11), Size(-1,-1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            image_points.push_back(corners);
            object_points.push_back(obj);
            printf("Snap stored!\n");
            successes++;
        }

        Mat resized_img;
        resize(img, resized_img, Size(640, 480)); // Example sizes
        imshow("win1", resized_img);

        Mat resized_gray;
        resize(gray, resized_gray, Size(640, 480)); // Example sizes
        imshow("win2", resized_gray);

        waitKey(1000);
    }
    cout << "Complete!" << "\n";

    Mat intrinsic = Mat(3,3, CV_32FC1);
    Mat distCoeffs;
    vector<Mat> rvecs;
    vector<Mat> tvecs;

    intrinsic.ptr<float>(0)[0] = 1;
    intrinsic.ptr<float>(1)[1] = 1;


    calibrateCamera(object_points, image_points, img.size(), intrinsic, distCoeffs, rvecs, tvecs);
    Mat imageUndistorted;

    cout << "==================Intrinsic Parameter=====================" << "\n";

    for( int i = 0; i < intrinsic.rows; i++) {
        for(int j = 0; j < intrinsic.cols; j++) {
            cout << intrinsic.at<double>(i, j)<<"\t\t";
        }
        cout << endl;
    }
    cout << "====================================================" << "\n";
    cap.release();
    waitKey();
    return 0;
    

}