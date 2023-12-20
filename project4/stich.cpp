#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

// Function to stitch two images
Mat stitchImages(Mat& img1, Mat& img2) {
    // Detect ORB features in both images
    Ptr<ORB> orb = ORB::create();
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    orb->detectAndCompute(img1, Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(img2, Mat(), keypoints2, descriptors2);

    // Convert descriptors to CV_32F for FLANN
    if(descriptors1.type() != CV_32F) {
        descriptors1.convertTo(descriptors1, CV_32F);
    }
    if(descriptors2.type() != CV_32F) {
        descriptors2.convertTo(descriptors2, CV_32F);
    }

    // Match the features using FLANN matcher
    FlannBasedMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    // Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.7f;
    vector<DMatch> good_matches;
    for (size_t i = 0; i < matches.size() - 1; i++) { // Added -1 to prevent out-of-bounds access
        if (matches[i].distance < ratio_thresh * matches[i+1].distance) {
            good_matches.push_back(matches[i]);
        }
    }

    // Extract location of good matches
    vector<Point2f> points1, points2;
    for (size_t i = 0; i < good_matches.size(); i++) {
        points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
        points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }

    // Find homography
    Mat H = findHomography(points2, points1, RANSAC);

    // Warp image
    Mat result;
    warpPerspective(img2, result, H, Size(img1.cols + img2.cols, img1.rows));
    Mat half(result, Rect(0, 0, img1.cols, img1.rows));
    img1.copyTo(half);

    return result;
}


int main() {
    // Load images
    Mat image1 = imread("pano1.jpg");
    Mat image2 = imread("pano2.jpg");
    Mat image3 = imread("pano3.jpg");
    Mat image4 = imread("pano4.jpg");

    // Resize images if needed
    // ...
    resize(image1,image1, Size(360,480));
    resize(image2,image2, Size(360,480));
    resize(image3,image3, Size(360,480));
    resize(image4,image4, Size(360,480));

    // Stitch images
    Mat result12 = stitchImages(image1, image2);
    Mat result123 = stitchImages(result12, image3);
    Mat finalResult = stitchImages(result123, image4);

    // Show the result
    imshow("Panoramic Image", finalResult);
    waitKey(0);

    return 0;
}
