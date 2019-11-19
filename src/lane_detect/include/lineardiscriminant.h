#ifndef LINEARDISCRIMINANT_H
#define LINEARDISCRIMINANT_H

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>

#include <ros/ros.h>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace cv;


class LinearDiscriminant
{
    public:
        LinearDiscriminant();
        ~LinearDiscriminant();

        void load_images( const String & dirname, vector< Mat > & img_lst);
        void sample_neg( const vector< Mat > & full_neg_lst, vector< Mat > & neg_lst, const Size & size );
        void computeHOGs( const Size wsize, const vector< Mat > & img_lst, vector< Mat > & gradient_lst, bool use_flip );
        void train(const Mat &trainData, const Mat &trainLabels);
        int predict(Mat &feature);
    private:
        Mat mean;
        Mat eigenvectors;
        Mat labels;
        Mat projections;
};

#endif