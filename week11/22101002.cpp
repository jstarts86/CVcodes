#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;




int main() {
    int currentFrame = 0, startFrame = 0;
    Mat input, originalFrame;
    int fps;
    int delay;
    CascadeClassifier face_classifier;
    Mat frame, grayframe;
    vector<Rect> faces, faces_f,faces_m,faces_n;

    int i;

    VideoCapture cap;

    if (!cap.open("Faces.mp4")) {
        cout << "Failed to open the video file!" << endl;
        waitKey(0);
        return -1; 
    }
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000/fps;
    face_classifier.load("/Users/john/ComputerVision/opencv-3.4.2/data/haarcascades/haarcascade_frontalface_alt.xml");
    char choice = 'r';
    bool pressT = false;
    bool pressR = true;
    bool pressF = false;
    bool pressM = false;
    bool pressN = false;
    Point lb, tr;
    Mat result, bgdModel, fgdModel, image, foreground;

    while (true) {
        cap >> frame;
        cvtColor(frame, grayframe, COLOR_BGR2GRAY);

        if (pressR == true) {
            imshow("Face Detection", frame); 
        }

        
        else if (!pressT && (choice == 'n'|| 'm' || 'f')) {
           //putText(frame, "Detect before tracking", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
            imshow("Face Detection", frame);
        }
        
        if(choice =='f' && pressT) {
                // farthest
            pressF = true;
            pressM = false;
            pressN = false;
            pressR = false;
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 4, 0, Size(31,31), Size(39,39));
            for (i = 0; i < faces.size(); i++) {
                Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

                Point tr(faces[i].x, faces[i].y);

                rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
            }  
            imshow("Face Detection", frame);
        }
        if(choice == 'm' && pressT) {
            //medium
            pressF = false;
            pressM = true;
            pressN = false;
            pressR = false;
            face_classifier.detectMultiScale(grayframe,faces, 1.1, 4, 0, Size(53, 53), Size(60,60));
            for (i = 0; i < faces.size(); i++) {
                Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

                Point tr(faces[i].x, faces[i].y);

                rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
            }  
            imshow("Face Detection", frame); 
        } if(choice == 'n' && pressT) {
            //nearest
            face_classifier.detectMultiScale(grayframe, faces, 1.1, 4, 0, Size(75,75), Size(90,90));
            pressF = false;
            pressM = false;
            pressN = true;
            pressR = false;
            for (i = 0; i < faces.size(); i++) {
                Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

                Point tr(faces[i].x, faces[i].y);

                rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
            }  
            imshow("Face Detection", frame); 

        }
        else if (choice == 't' && (!pressM && !pressF && !pressN )) {
            putText(frame, "Detect before tracking", Point(50, 80), FONT_HERSHEY_SIMPLEX, 1,Scalar(0, 200, 200), 4);
            pressR = false;
            pressT = true;
            imshow("Face Detection", frame);
        }
        char key = (char)waitKey(30);
        if (key == 27) break; 
        if (key == 't') {
            choice = key;
            pressT = true;
            pressR = false;
        }
        if (key == 'r') {
            choice = key;
            pressR = true;
            pressT = false;
            pressF = false;
            pressM = false;
            pressN = false;
        }
        if (key == 'n' || key == 'f' || key == 'm') {
            choice = key;
            pressR = false;
        }

    }
    return 0;
}
