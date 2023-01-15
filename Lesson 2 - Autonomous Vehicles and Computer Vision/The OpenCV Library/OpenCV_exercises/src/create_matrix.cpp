#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


using namespace std;

void createMatrix1()
{
    // create matrix
    int nrows = 480, ncols = 640;
    cv::Mat m1_8u;
    m1_8u.create(nrows, ncols, CV_8UC1); // two-channel matrix with 8bit unsigned elements
    m1_8u.setTo(cv::Scalar(255));        // white

    // STUDENT TASK :
    // Create a variable of type cv::Mat* named m3_8u which has three channels with a
    // depth of 8bit per channel. Then, set the first channel to 255 and display the result.
    /* 3 plane matrix */
    cv::Mat m3_8u;
    m3_8u.create(nrows, ncols, CV_8UC3);
    m3_8u.setTo(cv::Scalar(255,0,0)); // blue
    m3_8u.at<uint8_t>(277,226) = 69;
    m3_8u.at<uint8_t>(277,227) = 69;
    m3_8u.at<uint8_t>(277,228) = 69;
    uint8_t *hello = &m3_8u.at<uint8_t>(277,226);
    for(int i = 0;i < 3;i++)
    std::cout<<int(hello[i])<< " ";
    // show result
    string windowName = "First steps in OpenCV (m1_8u)";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, m1_8u);
    cv::waitKey(0); // wait for keyboard input before continuing

    // STUDENT TASK :
    // Display the results from the STUDENT TASK above
    string windowName2 = "Second steps in OpenCV (m3_8u)";
    cv::namedWindow(windowName2, 1); // create window
    cv::imshow(windowName2, m3_8u);
    cv::waitKey(0); // wait for keyboard input before continuing
}


int main()
{
    createMatrix1();
    return 0;
}