#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void magnitudeSobel()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");

    // convert image to grayscale
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // apply smoothing using the GaussianBlur() function from the OpenCV
    // ToDo : Add your code here
    cv::Mat imgFiltered;
    cv::GaussianBlur(imgGray,imgFiltered, cv::Size(3,3), 2);
    // create filter kernels using the cv::Mat datatype both for x and y
    // ToDo : Add your code here
    // To create filter kernels do the following !!!
    
    /* Use the following as an example to create a kernel line 28 to line 33 */
    // create filter kernels
    float sobel_x[9] = {-1, 0, +1, -2, 0, +2, -1, 0, +1};
    cv::Mat kernelX = cv::Mat(3, 3, CV_32F, sobel_x);

    float sobel_y[9] = {-1, -2, -1, 0, 0, 0, +1, +2, +1};
    cv::Mat kernelY = cv::Mat(3, 3, CV_32F, sobel_y);

    // create filter kernel
    /*float gauss_data[25] = {1, 4, 7, 4, 1,
                            4, 16, 26, 16, 4,
                            7, 26, 41, 26, 7,
                            4, 16, 26, 16, 4,
                            1, 4, 7, 4, 1};

    cv::Mat kernelX = cv::Mat(5, 5, CV_32F, gauss_data);
    cv::Mat kernelY = cv::Mat(5, 5, CV_32F, gauss_data);
    */
    // apply filter using the OpenCv function filter2D()
    // ToDo : Add your code here
    cv::Mat result_x, result_y;
    cv::filter2D(imgFiltered, result_x, -1, kernelX, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    cv::filter2D(imgFiltered, result_y, -1, kernelY, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
    // compute magnitude image based on the equation presented in the lesson 
    // ToDo : Add your code here
    cv::Mat magnitude = imgFiltered.clone();
    for (int r = 0; r < magnitude.rows; r++)
    {
        for (int c = 0; c < magnitude.cols; c++)
        {
            magnitude.at<uint8_t>(r, c) = sqrt(pow(result_x.at<uint8_t>(r, c), 2) +
                                                     pow(result_y.at<uint8_t>(r, c), 2));
        }
    }


    // show result
    string windowName = "Gaussian Blurring";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, magnitude);
    cv::waitKey(0); // wait for keyboard input before continuing
}

int main()
{
    magnitudeSobel();
}