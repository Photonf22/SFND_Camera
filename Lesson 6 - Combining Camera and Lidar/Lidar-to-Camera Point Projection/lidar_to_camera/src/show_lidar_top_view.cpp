#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <unordered_set>
#include "structIO.hpp"

using namespace std;

void showLidarTopview()
{
    float A = 0;
	 float B = 0;
	 float C = 0;
	 float D = 0;
	int inliers = 0;
	float temp_distance = 0;
	int max_distance = 0;
	int q = 0;
	int point2 =0;
	int point1 =0;
	int point3 = 0;
    int maxIterations = 100;
    float distanceTol =  0.8797210156;
    std::vector<LidarPoint> lidarPoints;
    std::vector<LidarPoint> lidarPointsOutliers;
    std::vector<LidarPoint> lidarPointsInliers;
    std::unordered_set<int> inliersResult;
    readLidarPts("../dat/C51_LidarPts_0000.dat", lidarPoints);

    cv::Size worldSize(10.0, 20.0); // width and height of sensor field in m
    cv::Size imageSize(1000, 2000); // corresponding top view image in pixel

    // create topview image
    cv::Mat topviewImg(imageSize, CV_8UC3, cv::Scalar(0, 0, 0));
    while(q < maxIterations)
	{
		std::unordered_set<int> temp;
	 	point1 = rand() & lidarPoints.size();
		while(1)
		{
			point2 = rand() & lidarPoints.size();
			if(point2 != point1)
			{
				break;
			}
		}
		while(1)
		{
			point3 = rand() & lidarPoints.size();
			if(point3 != point1 && point3 != point2)
			{
				break;
			}
		}
		float i = (lidarPoints[point2].y - lidarPoints[point1].y)*(lidarPoints[point3].z - lidarPoints[point1].z) -
				(lidarPoints[point2].z - lidarPoints[point1].z)*(lidarPoints[point3].y - lidarPoints[point1].y);
		
		float j = (lidarPoints[point2].z - lidarPoints[point1].z)*(lidarPoints[point3].x - lidarPoints[point1].x) -
				(lidarPoints[point2].x - lidarPoints[point1].x)*(lidarPoints[point3].z - lidarPoints[point1].z);
		
		float k = (lidarPoints[point2].x - lidarPoints[point1].x)*(lidarPoints[point3].y - lidarPoints[point1].y) -
				(lidarPoints[point2].y - lidarPoints[point1].y)*(lidarPoints[point3].x - lidarPoints[point1].x);

		A = i;
		B = j ;
		C = k;
		D= (i*lidarPoints[point1].x + j*lidarPoints[point1].y + k*lidarPoints[point1].z);
		//iterating through every point and getting the distance of the point to the line if below threshold then its an inlier
		
		for(int p = 0;p < lidarPoints.size();  p++)
		{
			temp_distance = fabs(A*lidarPoints[p].x + B*lidarPoints[p].y + C*lidarPoints[p].z - D)/sqrt(A*A + B*B + C*C);
			if(temp_distance <= distanceTol)
			{
				temp.insert(p);
			}
		}
		if(inliersResult.size() < temp.size())
		{
			inliersResult = temp;
		}
		q++;
	}
    int k = 0;
    // plot Lidar points into image
    for (auto it = lidarPoints.begin(); it != lidarPoints.end(); ++it)
    {
        
        float xw = (*it).x; // world position in m with x facing forward from sensor
        float yw = (*it).y; // world position in m with y facing left from sensor

        int y = (-xw * imageSize.height / worldSize.height) + imageSize.height;
        int x = (-yw * imageSize.width / worldSize.width) + imageSize.width / 2;

        // TODO: 
        // 1. Change the color of the Lidar points such that 
        // X=0.0m corresponds to red while X=20.0m is shown as green.
        float zw = (*it).z; // up and down
        //cout<< zw << endl;
        int red = min(255, (int)(255*abs((xw - worldSize.height)/worldSize.height)));
        int green =  min(255, (int)(255*(1- abs((xw - worldSize.height)/worldSize.height))));
        if(inliersResult.count(k))
        {
            cv::circle(topviewImg, cv::Point(x, y), 5, cv::Scalar(0, green, red), -1);
        }
        
        // RANSAC
        // 2. Remove all Lidar points on the road surface while preserving 
        // measurements on the obstacles in the scene.
        k++;
    }
   
    //std::cout<<topviewImg.channels() <<endl;
    //cout<<topviewImg<<endl;
    // plot distance markers
    float lineSpacing = 2.0; // gap between distance markers
    int nMarkers = floor(worldSize.height / lineSpacing);
    for (size_t i = 0; i < nMarkers; ++i)
    {
        int y = (-(i * lineSpacing) * imageSize.height / worldSize.height) + imageSize.height;
        cv::line(topviewImg, cv::Point(0, y), cv::Point(imageSize.width, y), cv::Scalar(255, 0, 0));
    }

    // display image
    string windowName = "Top-View Perspective of LiDAR data";
    cv::namedWindow(windowName, 2);
    cv::imshow(windowName, topviewImg);
    cv::waitKey(0); // wait for key to be pressed
}

int main()
{
    showLidarTopview();
}