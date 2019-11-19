#ifndef FEATUREEXTRACT_H
#define FEATUREEXTRACT_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

#include <ros/ros.h>
#include <vector>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <opencv2/ml.hpp>

using namespace cv::ml;
using namespace std;
using namespace cv;

class FeatureExtract
{
    public:
        FeatureExtract();
        ~FeatureExtract();

        static Size imageSize;
        static Size winSize;
        static Size blockSize;
        static Size stride;
        static Size cellSize;

        void computeHOGs(const vector<Mat>& img_lst, vector<Mat>& gradient_lst);
        void load_images(const String& dirname, vector< Mat > & img_lst);
        void convert_to_ml(const vector<Mat>& train_samples, Mat& trainData);
        vector<float> get_svm_detector( const Ptr< SVM >& svm );
    private:
        
};

#endif