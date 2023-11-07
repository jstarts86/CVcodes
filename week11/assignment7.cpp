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

    // open the webcam
    VideoCapture cap;

    // check if we succeeded
    if (!cap.open("Faces.mp4")) {
        cout << "Failed to open the video file!" << endl;
        waitKey(0);
        return -1; // Exit the program
    }
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000/fps;
    // face detection configuration
    face_classifier.load("/Users/john/ComputerVision/opencv-3.4.2/data/haarcascades/haarcascade_frontalface_alt.xml");
    char choice;
    while (true) {
        // get a new frame from webcam
        cap >> frame;
        // convert captured frame to gray scale
        cvtColor(frame, grayframe, COLOR_BGR2GRAY);

        if(choice == 't') {
            if(choice =='f') {
                 // farthest
                face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(35,35), Size(39,39));
                for (i = 0; i < faces.size(); i++) {
                    Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

                    Point tr(faces[i].x, faces[i].y);
                    
                    rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
                }
                imshow("Face Detection", frame);
            }
            else if(choice == 'm') {
            //medium
            face_classifier.detectMultiScale(grayframe,faces, 1.1, 3, 0, Size(55, 55), Size(55,55));
            }
        }
        

        //nearest
        //face_classifier.detectMultiScale(grayframe, faces, 1.1, 3, 0, Size(70,70), Size(80,80));
        //draw the results
        for (i = 0; i < faces.size(); i++) {
            Point lb(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

            Point tr(faces[i].x, faces[i].y);
            
            rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
        }

       

        if (waitKey(33) == 27) 
            break; // ESC

        }
}