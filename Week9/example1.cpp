#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat query, image, descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches; //DMatch is descriptor match 
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k; 
    float nndr;
    query = imread("assets/query.jpg");
    image = imread("assets/input.jpg");
    resize(query, query, Size(640, 480)); 
    resize(image, image, Size(640, 480));
    if (query.empty() || image.empty()) return -1;
    //Compute ORB Features
    orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);
    orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);
    //KNN Matching(k-nearest neighbor matching)
    //Find best and second-best matches
    k = 2;
    matcher.knnMatch(descriptors1, descriptors2, matches, k);
    // Find out the best match is definitely better than the second-best match 
    nndr = 0.6f;
    for (i = 0; i < matches.size(); i++) {
        if (matches.at(i).size() == 2 && matches.at(i).at(0).distance <= nndr * matches.at(i).at(1).distance) {
            goodMatches.push_back(matches[i][0]);
        }
    }
    //Draws the found matches of keypoints from two images.
    drawMatches(query, keypoints1, image, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    if (goodMatches.size() < 4) { 
        cout << "Matching failed" << endl; return 0; 
    }
    imshow("imgMatches", imgMatches);
    waitKey(0); 
}
