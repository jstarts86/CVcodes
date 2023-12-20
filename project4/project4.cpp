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
    Mat homographyOneToTwo, homographyTwoToThree, homographyThreeToFour;
    DoublePoints pointsFromOneToTwo, pointsFromTwoToThree, pointsFromThreeToFour;
    image1 = imread("pano1.jpg");
    image2 = imread("pano2.jpg");
    image3 = imread("pano3.jpg");
    image4 = imread("pano4.jpg");
    // resize(image1,image1, Size(360,480));
    // resize(image2,image2, Size(360,480));
    // resize(image3,image3, Size(360,480));
    // resize(image4,image4, Size(360,480));


    matchedOneFromTwo = findImageFeatures(image1, image2);

    // homographyOneToTwo = findHomography(matchedOneFromTwo.keypoints1,matchedOneFromTwo.keypoints2);
    // warpPerspective(image1, image1, homographyOneFromTwo, Size(300,300));

    pointsFromOneToTwo = findPoints(matchedOneFromTwo);
    homographyOneToTwo = findHomography(pointsFromOneToTwo.points1,pointsFromOneToTwo.points2, RANSAC);
    //warpPerspective(image1, image1, homographyOneToTwo, Size(image1.cols, image1.rows));    
    Mat warpImage2;
    warpPerspective(image2, warpImage2, homographyOneToTwo, Size(image1.cols, image1.rows * 1.2));


    matchedTwoFromThree = findImageFeatures(image2, image3);
    pointsFromTwoToThree = findPoints(matchedTwoFromThree);
    homographyTwoToThree = findHomography(pointsFromTwoToThree.points1, pointsFromTwoToThree.points2, RANSAC);
    Mat warpImage3;
    warpPerspective(image3, warpImage3, homographyTwoToThree, Size(image2.cols, warpImage2.rows));

    matchedThreeFromFour = findImageFeatures(image3, image4);
    pointsFromThreeToFour = findPoints(matchedThreeFromFour);
    homographyThreeToFour = findHomography(pointsFromThreeToFour.points1, pointsFromThreeToFour.points2, RANSAC);
    Mat warpImage4;
    warpPerspective(image4, warpImage4, homographyThreeToFour, Size(image3.cols, warpImage3.rows));


    Mat fullImage(Size((image1.cols + warpImage2.cols + warpImage3.cols + warpImage4.cols), image1.rows), image1.type());

    // crop the first image
    Mat crop_image1 = image1(Rect(0, 0 , image1.cols, image1.rows));
    
    
    // For combining image one and two
    // Mat rectOne(imageOneTwo, Rect(0, 0, image1.cols, image1.rows));
    // Mat rectTwo(imageOneTwo, Rect(image1.cols, 0, image2.cols, image2.rows));
    
    int crop_image1_width, image1_height, crop_image2_width, image2_height, crop_image3_width, image3_height, image4_height, crop_image4_width;

    // Combining image one and warped image two
    crop_image1_width = crop_image1.cols;
    crop_image2_width = crop_image1_width + warpImage2.cols;
    crop_image3_width = crop_image2_width + warpImage3.cols;
    crop_image4_width = crop_image3_width + warpImage4.cols;
    Mat rectOne(fullImage, Rect(0, 0, crop_image1_width, image1.rows));
    Mat rectTwo(fullImage, Rect(crop_image1_width, 0, warpImage2.cols, image2.rows));
    Mat rectThree(fullImage, Rect(crop_image2_width, 0, warpImage3.cols, image3.rows));
    Mat rectFour(fullImage, Rect(crop_image3_width, 0, warpImage4.cols, image4.rows));

    // For cropped image 1
    // Mat rectOne(fullImage, Rect(0, 0, crop_image_one.cols, crop_image_one.rows));
    // Mat rectTwo(fullImage, Rect(crop_image_one.cols, 0, image2.cols, image2.rows));
    // crop_image_one.copyTo(rectOne);


    // copy the image on and two to the canvas
    crop_image1.copyTo(rectOne);
    warpImage2.copyTo(rectTwo);
    warpImage3.copyTo(rectThree);
    warpImage4.copyTo(rectFour);


    resize(fullImage,fullImage, Size(360,480));
    // show the canvas
    imshow("full image", fullImage);
    //imshow("1 and 2 image", imageOneTwo);
    //imshow("test1", image1);
    //imshow("test2", image2);
    waitKey(0);
}