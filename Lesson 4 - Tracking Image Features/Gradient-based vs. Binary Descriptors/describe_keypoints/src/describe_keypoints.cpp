#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;

void descKeypoints1()
{
    // load image from file and convert to grayscale
    cv::Mat imgGray;
    cv::Mat img = cv::imread("../images/img1.png");
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // BRISK detector / descriptor
    // this Feature Detector variable allows us to plug in many different types of detectors! BRISK, SIFT , and many others
    // it is convinient because we don't need to find the type of detector below when we call the detector function
    // it just prepares the data structure
    cv::Ptr<cv::FeatureDetector> detector = cv::BRISK::create();
    vector<cv::KeyPoint> kptsBRISK;

    double t = (double)cv::getTickCount();
    detector->detect(imgGray, kptsBRISK);
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    cout << "BRISK detector with n= " << kptsBRISK.size() << " keypoints in " << 1000 * t / 1.0 << " ms" << endl;

    cv::Ptr<cv::DescriptorExtractor> descriptor = cv::BRISK::create();
    cv::Mat descBRISK;
    t = (double)cv::getTickCount();
    descriptor->compute(imgGray, kptsBRISK, descBRISK);
    t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
    cout << "BRISK descriptor in " << 1000 * t / 1.0 << " ms" << endl;

    // visualize results
    cv::Mat visImage = img.clone();
    cv::drawKeypoints(img, kptsBRISK, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    string windowName = "BRISK Results";
    cv::namedWindow(windowName, 1);
    imshow(windowName, visImage);
    cv::waitKey(0);

    // TODO: Add the SIFT detector / descriptor, compute the 
    // time for both steps and compare both BRISK and SIFT
    // with regard to processing speed and the number and 
    // visual appearance of keypoints.
    cv::Ptr<cv::FeatureDetector> detector2 = cv::SIFT::create();

    vector<cv::KeyPoint> kptsSIFT;

    double t2 = (double)cv::getTickCount();
    detector2->detect(imgGray, kptsSIFT);
    t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
    cout << "SIFT detector with n= " << kptsSIFT.size() << " keypoints in " << 1000 * t2 / 1.0 << " ms" << endl;

    cv::Ptr<cv::DescriptorExtractor> descriptor2 = cv::SIFT::create();
    
    cv::Mat descSIFT;
    t2 = (double)cv::getTickCount();
    descriptor2->compute(imgGray, kptsBRISK, descSIFT);
    t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
    cout << "SIFT descriptor in " << 1000 * t2 / 1.0 << " ms" << endl;

    // visualize results
    cv::Mat visImage2 = img.clone();
    cv::drawKeypoints(img, kptsSIFT, visImage2, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    string windowName2 = "SIFT Results";
    cv::namedWindow(windowName2, 1);
    imshow(windowName2, visImage2);
    cv::waitKey(0);
    

}

int main()
{
    descKeypoints1();
    return 0;
}