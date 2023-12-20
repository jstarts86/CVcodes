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



Mat makePanorama(Mat image1, Mat image2) {
    FeatureMatching matchedOneFromTwo = findImageFeatures(image1, image2);
    DoublePoints pointsFromOneToTwo = findPoints(matchedOneFromTwo);

    Mat homographyOneToTwo = findHomography(pointsFromOneToTwo.points2, pointsFromOneToTwo.points1, RANSAC);

	Mat cut;
    // Mat warpImage2;
    // warpPerspective(image2, warpImage2, homographyOneToTwo, Size(image1.cols, image1.rows));

    // Mat panorama(Size(image1.cols + warpImage2.cols, max(image1.rows, warpImage2.rows)), image1.type());
    
    // Copy images to panorama canvas
    // Mat left(panorama, Rect(0, 0, image1.cols, image1.rows));
    // image1.copyTo(left);
    // Mat right(panorama, Rect(image1.cols, 0, warpImage2.cols, warpImage2.rows));
    // warpImage2.copyTo(right);
    Mat result;
    warpPerspective(image2, result, homographyOneToTwo, Size(image1.cols * 2, image1.rows * 1.5));
    //imshow("result", result);
    Mat half(result, Rect(0, 0, image1.cols, image1.rows));
    image1.copyTo(half);

    // return result;
    int color_at_y = 0;
    int color_at_x = 0;
	for (int y = 0; y < result.rows; y++) {
		for (int x = 0; x < result.cols; x++) {
			if (result.at<Vec3b>(y, x)[0] == 0 && result.at<Vec3b>(y, x)[1] == 0 && result.at<Vec3b>(y, x)[2] == 0) {
				continue;
			}
			if (color_at_x < x) {
				color_at_x = x;
			}
			if (color_at_y < y){
				color_at_y = y;
			}
		}
	}
	
	cut = result(Range(0, color_at_y), Range(0, color_at_x));
	return cut;
}

int main() {
    Mat image1, image2, image3, image4;
    Mat result1, result2, result3, result4,result5, result6;
    image1 = imread("panorama_image/pano1.jpg");
    image2 = imread("panorama_image/pano2.jpg");
    image3 = imread("panorama_image/pano3.jpg");
    image4 = imread("panorama_image/pano4.jpg");
    // resize(image1,image1, Size(240,426));
    // resize(image2,image2, Size(240,426));
    // resize(image3,image3, Size(240,426));
    // resize(image4,image4, Size(240,426));
    resize(image1,image1, Size(360,480));
    resize(image2,image2, Size(360,480));
    resize(image3,image3, Size(360,480));
    resize(image4,image4, Size(360,480));
    // result1 = makePanorama(image1, image2);
    // result2 = makePanorama(image3, image4);
    // result3 = makePanorama(result1, result2);

    result1 = makePanorama(image1, image2);
    result2 = makePanorama(result1, image3);
    result3 = makePanorama(result2, image4);
    //result4 = makePanorama(image3, image4);
    //result5 = makePanorama(result3, result4);
    // result6 = makePanorama(result5, image4);
    // result4 = makePanorama(result3, image4);
    // resize(result1, result1, Size(360,480));
    //resize(image1,image1, Size(360,480));
    //resize(image1, image1, Size(1080,1980));
    //imshow("image1", image1);
    //resize(result1, result1, Size(1080,1980));
    //imshow("result1", result1);
    //resize(result2, result2, Size(360,480));
    //resize(result2, result2, Size(1080,1980));
    //imshow("result2", result2);
    //resize(result3, result3, Size(1080,1980));
    //resize(result2, result2, Size(360,480));
    imshow("panorama_img", result3);
    //imshow("result4", result4);
    //imshow("result5", result5);
    // imshow("result6", result6);
    







    //imshow("full image", fullImage);
    //imshow("1 and 2 image", imageOneTwo);
    //imshow("test1", image1);
    //imshow("test2", image2);
    waitKey(0);
}