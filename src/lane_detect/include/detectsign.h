#ifndef DETECTSIGN_H
#define DETECTSIGN_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cv;

class DetectSign 
{
    public:
        DetectSign();
        ~DetectSign();
        void detectTrafficSign(Mat &img);
        void setTrafficSignInfo(int trafficSignInfo);
        int getTrafficSignInfo();
        void setPrevTrafficSignInfo(int prevTrafficSignInfo);
        int getPrevTrafficSignInfo();
    private:
        int trafficSignInfo;
        int prevTrafficSignInfo;
        Size winSize;
        Size blockSize;
        Size stride;
        Size cellSize;

        Mat preProcess(const Mat &img);
        vector<float> computeHOG(const Mat &img);
        int regconizeTrafficSign(Mat cropImg);
        void trackFilteredTrafficSign(Mat threshold, Mat blur);
};

#endif