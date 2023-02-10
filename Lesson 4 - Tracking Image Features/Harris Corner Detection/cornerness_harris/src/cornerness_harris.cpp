#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
std::vector<cv::KeyPoint> NMS_Algorithm(cv::Mat CornerHarris_img, int apertureSize, int threshold)
{
    // harris response matrix with all maximum pixels
    double maxOverlap = 0.0; // max. permissible overlap between two features in %, used during non-maxima suppression
    std::vector<cv::KeyPoint> result;
    for(int i = 0;i < CornerHarris_img.rows ;i++)
    {
        for(int j = 0;j < CornerHarris_img.cols ;j++)
        {
            int pixel_intensity = (int)CornerHarris_img.at<float>(i,j);

            if(pixel_intensity > threshold)
            {
                cv::KeyPoint new_key;
                new_key.pt = cv::Point2f(i,j);
                new_key.size = 2*apertureSize;
                new_key.response = pixel_intensity;
            
            bool added = false;
            for(std::vector<cv::KeyPoint>::iterator iter= result.begin();iter != result.end();iter++)
            {  
                //check if overlap and % of overlap
                double percentage_overlap = cv::KeyPoint::overlap(new_key, *iter);
                if(percentage_overlap > maxOverlap)
                {
                    added = true;
                    // check which has more intensity/ response
                    if(new_key.response > iter->response)
                    {
                        *iter = new_key;
                        break;
                    }
                }
                
            }
            if (!bOverlap)
                {                                     // only add new key point if no overlap has been found in previous NMS
                    keypoints.push_back(newKeyPoint); // store new keypoint in dynamic list
                }
            }
        }
    }
    return result;
}
void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);
    
    // visualize results
    string windowName = "Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, dst_norm_scaled);
    cv::waitKey(0);

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perform a non-maximum suppression (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.
    std::vector<cv::KeyPoint> keypoints;
    keypoints = NMS_Algorithm(dst_norm_scaled,apertureSize,minResponse);
     // visualize keypoints
    windowName = "Harris Corner Detection Results";
    cv::namedWindow(windowName, 5);
    cv::Mat visImage = dst_norm_scaled.clone();
    cv::drawKeypoints(dst_norm_scaled, keypoints, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::imshow(windowName, visImage);
    cv::waitKey(0);
    // EOF STUDENT CODE
}

int main()
{
    cornernessHarris();
}