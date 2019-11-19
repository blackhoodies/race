#include "detectsign.h"
#include "lineardiscriminant.h"

DetectSign::DetectSign() {}
DetectSign::~DetectSign() {}

Mat DetectSign::preProcess(const Mat &img) 
{
    return img;
}

vector<float> DetectSign::computeHOG(const Mat &img)
{
    HOGDescriptor hog(
        winSize, 
        blockSize, 
        stride, 
        cellSize,
        9,
        1,
        -1,
        0,
        0.2,
        1,
        64,
        1);

    vector<float> descriptor;
    hog.compute(img, descriptor);

    return descriptor;
}

Mat getHOGDescriptor(const Mat& img, vector<float>& descriptor, const Size& size)
{

    
}

int regconizeTrafficSign(Mat& img)
{
    int windows_n_rows = 80;
    int windows_n_cols = 80;
    int StepSlide = 80;
    for (int row = 0; row <= img.rows - windows_n_rows; row += StepSlide)
    {
        for (int col = 0; col <= img.cols - windows_n_cols; col += StepSlide)
            {
                    Rect windows(col, row, windows_n_rows, windows_n_cols);
                    Mat Roi = img(windows);
                    
                }
            }

}

