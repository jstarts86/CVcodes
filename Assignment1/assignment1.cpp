#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;


int main() {
    Mat frame;
    VideoCapture cap;
    int fps;
    int delay;
    if (cap.open("background.mp4") == 0 ){
        cout << "no such file!" << endl;
       waitKey(0);
    }

    fps = cap.get(CAP_PROP_FPS); 
    delay = 1000 /fps;

    for(int i = 0; i <= 72; i++){
        cap >> frame;
        
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }
        imshow("video", frame);
    
        waitKey(delay);
    }
    
    double total_frame = cap.get(CAP_PROP_FRAME_COUNT);
    double time_in_msec = cap.get(CAP_PROP_POS_MSEC);
    int current_frame = cap.get(CAP_PROP_POS_FRAMES);
    printf("fps is %d \ntotal frame count is %f\ntime in msec is %f\n Current frame is %d", fps,total_frame,time_in_msec,current_frame);

    // while(1) {
    //     cap >> frame;

    //     if (frame.empty()) {
    //         cout << "end of video" << endl;
    //         break;
    //     }
    //     imshow("video", frame);

    //     waitKey(delay);
    // }



}
