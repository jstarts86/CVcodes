#include  <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;



// images step by step
// 	1. Extract the features of the two images entered
// 	2. Compare and match the features between entered images
// 	3. Make a homography matrix using matching points between images 
// 	4. Convert the image through the created homograph matrix
// 	5. Connects the converted images
// 	6. Repeat the above process when a new image comes in
struct FeatureMatching {
    vector<KeyPoint> keypoints1;
    vector<KeyPoint> keypoints2;
    vector<DMatch> goodMatches;
};
struct DoublePoints{
    vector<Point2f> points1;
    vector<Point2f> points2;
};
DoublePoints findPoints(FeatureMatching matchedImageFeautures) {
    DoublePoints result;
    for (size_t i = 0; i < matchedImageFeautures.goodMatches.size(); i++) {
        result.points1.push_back(matchedImageFeautures.keypoints1[matchedImageFeautures.goodMatches[i].queryIdx].pt);
        result.points2.push_back(matchedImageFeautures.keypoints2[matchedImageFeautures.goodMatches[i].trainIdx].pt);
    }
    return result;
    

}
FeatureMatching findImageFeatures(Mat query, Mat image) {
    FeatureMatching result;
    Mat  descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector< vector< DMatch> > matches; //DMatch is descriptor match 
    vector< DMatch > goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;
    int i, k; 
    float nndr;
    // if (query.empty() || image.empty()) {
    //     return a;
    // } 
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
    result.goodMatches = goodMatches;
    result.keypoints1 = keypoints1;
    result.keypoints2 = keypoints2;
    return result;
}



int main() {
    Mat image1, image2, image3, image4;
    FeatureMatching matchedOneFromTwo, matchedTwoFromThree, matchedThreeFromFour;
    Mat homographyOneFromTwo;
    image1 = imread("pano1.jpg");
    image2 = imread("pano2.jpg");
    image3 = imread("pano3.jpg");
    image4 = imread("pano4.jpg");
    resize(image1,image1, Size(480,640));
    resize(image2,image2, Size(480,640));
    resize(image3,image3, Size(480,640));
    resize(image4,image4, Size(480,640));
    matchedOneFromTwo = findImageFeatures(image1, image2);

    // homographyOneFromTwo = findHomography(matchedOneFromTwo.keypoints1,matchedOneFromTwo.keypoints2);
    // warpPerspective(image1, image1, homographyOneFromTwo, Size(300,300));

    DoublePoints pointsFromOnetoTwo = findPoints(matchedOneFromTwo);
    homographyOneFromTwo = findHomography(pointsFromOnetoTwo.points1,pointsFromOnetoTwo.points2, RANSAC);
    //warpPerspective(image1, image1, homographyOneFromTwo, Size(image1.cols, image1.rows));
    Mat warpImage2;
    warpPerspective(image2, warpImage2, homographyOneFromTwo, Size(image1.cols + image2.cols, image2.rows));

    Mat fullImage(Size((image1.cols + image2.cols + image3.cols + image4.cols), image1.rows), image1.type());
    Mat imageOneTwo(Size((image1.cols + image2.cols), image1.rows), image1.type());
    // Mat rectOne(imageOneTwo, Rect(0, 0, image1.cols, image1.rows));
    // Mat rectTwo(imageOneTwo, Rect(image1.cols, 0, image2.cols, image2.rows));
    Mat rectOne(fullImage, Rect(0, 0, image1.cols, image1.rows));
    Mat rectTwo(fullImage, Rect(image1.cols, 0, image2.cols, image2.rows));
    image1.copyTo(rectOne);
    warpImage2.copyTo(rectTwo);
    imshow("full image", fullImage);
    //imshow("1 and 2 image", imageOneTwo);
    //imshow("test1", image1);
    //imshow("test2", image2);
    waitKey(0);
}