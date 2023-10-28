#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
int getAmountofMatches(Mat query, Mat comparison);

Mat getMatchedImage(Mat query, Mat image);


Mat getMatchedImage(Mat query, Mat image) {
    Mat  descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches; //DMatch is descriptor match 
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k; 
    float nndr;
    resize(query, query, Size(640, 480)); 
    resize(image, image, Size(640, 480));
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
    drawMatches(query, keypoints1, image, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    //drawMatches(image, keypoints2, query, keypoints1, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    return imgMatches;
}
int getAmountofMatches(Mat query, Mat comparison){
    Mat  descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches; //DMatch is descriptor match 
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k; 
    float nndr;
    resize(query, query, Size(640, 480)); 
    resize(comparison, comparison, Size(640, 480));
    if (query.empty() || comparison.empty()) return -1;
    //Compute ORB Features
    orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);
    orbF->detectAndCompute(comparison, noArray(), keypoints2, descriptors2);
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
    return goodMatches.size();
}


int main() {
    Mat query, image, matchedImage;
    int match_array[10];
    String folderPath = "DBs/";
    String queryPath = "query_image/";

    char name[50]; 
    cout << "Enter query image name: ";
    cin >> name;
    String fullPath = queryPath + string(name);

    query = imread(fullPath);
    if (query.empty()) {
        cout << "No file!" << endl;
        return -1;
    }

    vector<String> image_path_names;
    glob(folderPath + "*.jpg", image_path_names, false);
    
    for (int i = 0; i < image_path_names.size(); i++) {
        int current_image_num = i + 1;
        image = imread(image_path_names[i]);
        // Find how much each image matches with the query and store it into an array
        match_array[i] = getAmountofMatches(image, query);

        // print out the number
        cout << "Image number " << current_image_num << " Matching: " << match_array[i] << endl;
        

    } 

    //Find out the highest matching image
    int champion, champion_index, challenger;
    champion = match_array[0];
    champion_index = 0;

    for(int i = 0; i < 10; i++) {
        challenger = match_array[i];
        if(challenger > champion) {
            champion = challenger;
            champion_index = i;
        }
    }


    // draw the matching lines greatest matching image
    image = imread(image_path_names[champion_index]);
    if (query.empty() || image.empty()) return -1;
    matchedImage = getMatchedImage(image, query);
    


    // imshow the query image
    resize(query, query, Size(640, 480));
    cout << "Match found!" << endl;
    imshow("Query", query);
    imshow("Best_matching", matchedImage);
    waitKey(0);
    return 0;
}