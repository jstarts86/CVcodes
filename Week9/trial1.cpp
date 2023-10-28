#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

int main() {
    Mat query, image, descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches;
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k; float nndr;
    string input;
    
    cout << "Enter query image name: ";
    cin >> input;
    
    query = imread(input);
    if (query.empty()) {
        cout << "No file!" << endl;
        return -1;
    }
    
    resize(query, query, Size(640, 480));
    //Compute ORB Features for query image
    orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);
    
    vector<String> filenames;
    glob("DBs", filenames, false);
    
    int bestMatchIndex = -1;
    size_t bestMatchCount = 0;
    
    for (size_t j = 0; j < filenames.size(); j++) {
        image = imread(filenames[j]);
        resize(image, image, Size(640, 480));
        orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);
        
        k = 2;
        matcher.knnMatch(descriptors1, descriptors2, matches, k);
        
        goodMatches.clear();
        nndr = 0.6f;
        for (i = 0; i < matches.size(); i++) {
            if (matches[i].size() == 2 && matches[i][0].distance <= nndr * matches[i][1].distance) {
                goodMatches.push_back(matches[i][0]);
            }
        }
        
        if (goodMatches.size() > bestMatchCount) {
            bestMatchCount = goodMatches.size();
            bestMatchIndex = j;
        }
    }
    
    if (bestMatchIndex == -1) {
        cout << "No matching image found" << endl;
        return -1;
    }
    
    image = imread(filenames[bestMatchIndex]);
    resize(image, image, Size(640, 480));
    orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);
    matcher.knnMatch(descriptors1, descriptors2, matches, k);
    
    goodMatches.clear();
    for (i = 0; i < matches.size(); i++) {
        if (matches[i].size() == 2 && matches[i][0].distance <= nndr * matches[i][1].distance) {
            goodMatches.push_back(matches[i][0]);
        }
    }
    
    drawMatches(query, keypoints1, image, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    imshow("Query", query);
    imshow("Best_matching", imgMatches);
    waitKey(0);
    
    return 0;
}
