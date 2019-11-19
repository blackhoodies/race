#include "featureextract.h"

Size FeatureExtract::imageSize(320,240);
Size FeatureExtract::winSize(320, 240);

FeatureExtract::FeatureExtract() {}
FeatureExtract::~FeatureExtract() {}

void FeatureExtract::computeHOGs(const vector<Mat>& img_lst, vector<Mat>& gradient_lst)
{
    //Only in Testing or in Simulator
    HOGDescriptor hog;
    hog.winSize = imageSize;
    vector<float> descriptors;
    Mat resizedImage;
    Mat gray;
    /*
    resize(image, resizedImage, FeatureExtract::imageSize);
    cvtColor(resizedImage, gray, COLOR_BGR2GRAY);
    hog.compute(gray, descriptors, Size(8,8), Size(0,0));
    gradient.push_back(Mat(descriptors).clone());
    */
    for( size_t i = 0 ; i < img_lst.size(); i++ )
    {
        resize(img_lst[i], resizedImage, FeatureExtract::imageSize);
        cvtColor( img_lst[i], gray, COLOR_BGR2GRAY );
        hog.compute( gray, descriptors, Size( 8, 8 ), Size( 0, 0 ) );
        gradient_lst.push_back( Mat( descriptors ).clone() );      
    }
}

void FeatureExtract::load_images( const String & dirname, vector< Mat > & img_lst)
{
    vector< String > files;
    glob( dirname, files );
    for ( size_t i = 0; i < files.size(); ++i )
    {
        Mat img = imread( files[i] ); // load the image
        
        if ( img.empty() )            // invalid image, skip it.
        {
            cout << files[i] << " is invalid!" << endl;
            continue;
        }

        resize(img, img, imageSize);
        img_lst.push_back(img);
    }
}

void FeatureExtract::convert_to_ml(const vector<Mat>& train_samples, Mat& trainData)
{
        
    const int rows = (int)train_samples.size();
    const int cols = (int)std::max( train_samples[0].cols, train_samples[0].rows );
    Mat tmp( 1, cols, CV_32FC1 ); //< used for transposition if needed
    trainData = Mat( rows, cols, CV_32FC1 );
    for( size_t i = 0 ; i < train_samples.size(); ++i )
    {
        CV_Assert( train_samples[i].cols == 1 || train_samples[i].rows == 1 );
        if( train_samples[i].cols == 1 )
        {
            transpose( train_samples[i], tmp );
            tmp.copyTo( trainData.row( (int)i ) );
        }
        else if( train_samples[i].rows == 1 )
        {
            train_samples[i].copyTo( trainData.row( (int)i ) );
        }
    }
}

vector<float> FeatureExtract::get_svm_detector( const Ptr< SVM >& svm )
{
    // get the support vectors
    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    // get the decision function
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction( 0, alpha, svidx );
    CV_Assert( alpha.total() == 1 && svidx.total() == 1 && sv_total == 1 );
    CV_Assert( (alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
               (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f) );
    CV_Assert( sv.type() == CV_32F );
    vector< float > hog_detector( sv.cols + 1 );
    memcpy( &hog_detector[0], sv.ptr(), sv.cols*sizeof( hog_detector[0] ) );
    hog_detector[sv.cols] = (float)-rho;
    return hog_detector;
}
