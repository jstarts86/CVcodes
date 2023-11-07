#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;




int main() {
    CascadeClassifier face_classifier;
    Mat frame, grayframe;
    vector<Rect> faces_f, faces_m, faces_n;

    int i;

    // open the webcam
    VideoCapture cap(0);

    // check if we succeeded
    if (!cap.isOpened()) {
        cout << "Could not open camera" << endl;
        return -1;
    }

    // face detection configuration
    face_classifier.load("/Users/john/ComputerVision/opencv-3.4.2/data/haarcascades/haarcascade_frontalface_alt.xml");
    
    while (1) {
        // get a new frame from webcam
        cap >> frame;
        // convert captured frame to gray scale
        cvtColor(frame, grayframe, COLOR_BGR2GRAY);
        face_classifier.detectMultiScale(grayframe, faces_f,1.1, 3, 0, Size(35,35), Size(45,45));
        face_classifier.detectMultiScale(grayframe,faces_m,
        1.1, // increase search scale by 10% each pass
        3, // merge groups of three detections
        0, // not used for a new cascade
        Size(45, 45),
        Size(60,60) //minimum size for detection
        );
        face_classifier.detectMultiScale(grayframe, faces_n,1.1, 3, 0, Size(35,35), Size(45,45));

        // draw the results
        for (i = 0; i < faces_f.size(); i++) {
            Point lb(faces_f[i].x + faces_f[i].width, faces_f[i].y + faces_f[i].height);

            Point tr(faces_f[i].x, faces_f[i].y);
            
            rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
        }

        // print the output
        imshow("Face Detection", frame);

        if (waitKey(33) == 27) 
            break; // ESC

        }
}