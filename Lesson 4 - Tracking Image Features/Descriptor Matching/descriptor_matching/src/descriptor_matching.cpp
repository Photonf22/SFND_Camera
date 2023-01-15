#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

#include "structIO.hpp"

using namespace std;

void matchDescriptors(cv::Mat &imgSource, cv::Mat &imgRef, vector<cv::KeyPoint> &kPtsSource, vector<cv::KeyPoint> &kPtsRef, cv::Mat &descSource, cv::Mat &descRef,
                      vector<cv::DMatch> &matches, string descriptorType, string matcherType, string selectorType, bool crossCheck)
{
    // configure matcher
    cv::Ptr<cv::DescriptorMatcher> matcher;

    if (matcherType.compare("MAT_BF") == 0)
    {
        int normType = descriptorType.compare("DES_BINARY") == 0 ? cv::NORM_HAMMING : cv::NORM_L2;
        matcher = cv::BFMatcher::create(normType, crossCheck);
        cout << "BF matching cross-check=" << crossCheck;
    }
    else if (matcherType.compare("MAT_FLANN") == 0)
    {
        if (descSource.type() != CV_32F)
        { // OpenCV bug workaround : convert binary descriptors to floating point due to a bug in current OpenCV implementation
            descSource.convertTo(descSource, CV_32F);
            descRef.convertTo(descRef, CV_32F);
        }
        //... TODO : implement FLANN matching
        matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
        cout << "FLANN matching";
    }

    // perform matching task
    if (selectorType.compare("SEL_NN") == 0)
    { // nearest neighbor (best match)

        double t = (double)cv::getTickCount();
        matcher->match(descSource, descRef, matches); // Finds the best match for each descriptor in desc1
        t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();
        cout << " (NN) with n=" << matches.size() << " matches in " << 1000 * t / 1.0 << " ms" << endl;
    }
    else if (selectorType.compare("SEL_KNN") == 0)
    { // k nearest neighbors (k=2)
        int k = 2;
        std::vector<std::vector<cv::DMatch>> knn_matches;
        vector<cv::DMatch> bad_matches;
        double t2 = (double)cv::getTickCount();
        // TODO : implement k-nearest-neighbor matching
        matcher->knnMatch(descSource,descRef,knn_matches, 2);
        t2 = ((double)cv::getTickCount() - t2) / cv::getTickFrequency();
        cout << " (NN) with n=" << matches.size() << " matches in " << 1000 * t2 / 1.0 << " ms" << endl;
        // TODO : filter matches using descriptor distance ratio test
        const float ratio_threshold = 0.8;
        for(size_t i = 0; i < knn_matches.size() ;i++)
        {
            if(knn_matches[i][0].distance < ratio_threshold * knn_matches[i][1].distance)
            {
                matches.push_back(knn_matches[i][0]);
            }
            else
            {
                bad_matches.push_back(knn_matches[i][0]);
            }
        }
        cout << " % of discarded matches: " << ((float)bad_matches.size()/ (float)(bad_matches.size() + matches.size())) * 100<< endl;

    }

    // visualize results
    cv::Mat matchImg = imgRef.clone();
    cv::drawMatches(imgSource, kPtsSource, imgRef, kPtsRef, matches,
                    matchImg, cv::Scalar::all(-1), cv::Scalar::all(-1), vector<char>(), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    string windowName = "Matching keypoints between two camera images (best 50)";
    cv::namedWindow(windowName, 7);
    cv::imshow(windowName, matchImg);
    cv::waitKey(0);
}

int main()
{
    bool CrossCheck = false;
    cv::Mat imgSource = cv::imread("../images/img1gray.png");
    cv::Mat imgRef = cv::imread("../images/img2gray.png");

    vector<cv::KeyPoint> kptsSourceSmall, kptsRefSmall; 
    readKeypoints("../dat/C35A5_KptsSource_BRISK_small.dat", kptsSourceSmall);
    readKeypoints("../dat/C35A5_KptsRef_BRISK_small.dat", kptsRefSmall);

    cv::Mat descSource_BS, descRef_BS; 
    readDescriptors("../dat/C35A5_DescSource_BRISK_small.dat", descSource_BS);
    readDescriptors("../dat/C35A5_DescRef_BRISK_small.dat", descRef_BS);

    vector<cv::KeyPoint> kptsSourceLarge, kptsRefLarge; 
    readKeypoints("../dat/C35A5_KptsSource_BRISK_large.dat", kptsSourceLarge);
    readKeypoints("../dat/C35A5_KptsRef_BRISK_large.dat", kptsRefLarge);

    cv::Mat descSource_BL, descRef_BL; 
    readDescriptors("../dat/C35A5_DescSource_BRISK_large.dat", descSource_BL);
    readDescriptors("../dat/C35A5_DescRef_BRISK_large.dat", descRef_BL);

    vector<cv::KeyPoint> kptsSIFTSource, kptsSIFTRef; 
    readKeypoints("../dat/C35A5_KptsSource_SIFT.dat", kptsSIFTSource);
    readKeypoints("../dat/C35A5_KptsRef_SIFT.dat", kptsSIFTRef);

    cv::Mat descSIFTSource, descSIFTRef; 
    readDescriptors("../dat/C35A5_DescSource_SIFT.dat", descSIFTSource);
    readDescriptors("../dat/C35A5_DescRef_SIFT.dat", descSIFTRef);

    vector<cv::DMatch> matches;
    string matcherType = "MAT_BF"; 
    string descriptorType = "DES_BINARY"; 
    string selectorType = "SEL_NN"; 

    // Brisk Small w/ cross-check OFF
    CrossCheck = false;
    matchDescriptors(imgSource, imgRef, kptsSourceSmall, kptsRefSmall, descSource_BS, descRef_BS, matches, descriptorType, matcherType, selectorType, CrossCheck);
    // Brisk Small w/ cross-check ON
    CrossCheck = true;
    matchDescriptors(imgSource, imgRef, kptsSourceSmall, kptsRefSmall, descSource_BS, descRef_BS, matches, descriptorType, matcherType, selectorType, CrossCheck);
    // FLANN Small using k-nearest neighbor matching and calculating % of removed matches -> using FLANN does not do crosscheck so crosscheck parameter value doesn't matter
    matcherType = "MAT_FLANN";
    selectorType = "SEL_KNN"; 
    matchDescriptors(imgSource, imgRef, kptsSourceSmall, kptsRefSmall, descSource_BS, descRef_BS, matches, descriptorType, matcherType, selectorType, CrossCheck);
    // FLANN Large using k-nearest neighbor matching and calculating % of removed matches -> using FLANN does not do crosscheck so crosscheck parameter value doesn't matter
    matchDescriptors(imgSource, imgRef, kptsSourceLarge, kptsRefLarge, descSource_BL, descRef_BL, matches, descriptorType, matcherType, selectorType, CrossCheck);
    
    /* BRISK large keypoints w BF*/
    // Brisk Large using k-nearest neighbor matching and calculating % of removed matches -> using FLANN does not do crosscheck so crosscheck parameter value doesn't matter
    matcherType = "MAT_BF";
    CrossCheck = true;
    selectorType = "SEL_NN"; 
    matchDescriptors(imgSource, imgRef, kptsSourceLarge, kptsRefLarge, descSource_BL, descRef_BL, matches, descriptorType, matcherType, selectorType, CrossCheck);
    /* BRISK large keypoints w FLANN*/
    // FLANN Large using k-nearest neighbor matching and calculating % of removed matches -> using FLANN does not do crosscheck so crosscheck parameter value doesn't matter
    matcherType = "MAT_FLANN";
    selectorType = "SEL_KNN"; 
    matchDescriptors(imgSource, imgRef, kptsSourceLarge, kptsRefLarge, descSource_BL, descRef_BL, matches, descriptorType, matcherType, selectorType, CrossCheck);

    /* SIFT keypoints w BF*/
    matcherType = "MAT_BF";
    descriptorType = "DES_HOG"; 
    CrossCheck = true;
    selectorType = "SEL_NN"; 
    matchDescriptors(imgSource, imgRef, kptsSIFTSource, kptsSIFTRef, descSIFTSource, descSIFTRef, matches, descriptorType, matcherType, selectorType, CrossCheck);
    /* SIFT keypoints w FLANN*/
    matcherType = "MAT_FLANN";
    selectorType = "SEL_KNN"; 
    matchDescriptors(imgSource, imgRef, kptsSIFTSource, kptsSIFTRef, descSIFTSource, descSIFTRef, matches, descriptorType, matcherType, selectorType, CrossCheck);
}