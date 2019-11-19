#include "featureextract.h"
#include "opencv2/ml.hpp"

using namespace cv::ml;

int main(int argc, char **argv)
{
    vector<Mat> pos_lst, neg_lst, gradient_lst;
    vector<int> labels;

    FeatureExtract* featureExtract = new FeatureExtract();

    clog << "Positive images are being loaded...";
    cv::String pos_dir = "/home/tristan/catkin_ws/src/lane_detect/src/images/positive";
    featureExtract->load_images(pos_dir, pos_lst);
    clog << "...[done]" << endl;

    clog << "Negative images are being loaded...";
    cv::String neg_dir = "/home/tristan/catkin_ws/src/lane_detect/src/images/negative";
    featureExtract->load_images(neg_dir, neg_lst);
    clog << "...[done]" << endl;

    clog << "Histogram of Gradients are being calculated for positive images...";
    featureExtract->computeHOGs(pos_lst, gradient_lst);
    size_t positive_count = gradient_lst.size();
    labels.assign(positive_count, +1);
    clog << "...[done] ( positive count : " << positive_count << " )" << endl;

    clog << "Histogram of Gradients are being calculated for negative images...";
    featureExtract->computeHOGs(neg_lst, gradient_lst);
    size_t negative_count = gradient_lst.size() - positive_count;
    labels.insert( labels.end(), negative_count, -1 );
    CV_Assert( positive_count < labels.size() );
    clog << "...[done] ( negative count : " << negative_count << " )" << endl;

    Mat train_data;
    featureExtract->convert_to_ml( gradient_lst, train_data );

    clog << "Training SVM...";
    Ptr< SVM > svm = SVM::create();
    /* Default values to train SVM */
    svm->setCoef0( 0.0 );
    svm->setDegree( 3 );
    svm->setTermCriteria( TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 1e-3 ) );
    svm->setGamma( 0 );
    svm->setKernel( SVM::LINEAR );
    svm->setNu( 0.5 );
    svm->setP( 0.1 ); // for EPSILON_SVR, epsilon in loss function?
    svm->setC( 0.01 ); // From paper, soft classifier
    svm->setType( SVM::EPS_SVR ); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task
    svm->train( train_data, ROW_SAMPLE, labels );
    clog << "...[done]" << endl;

    HOGDescriptor hog;
    hog.winSize = featureExtract->winSize;
    hog.setSVMDetector(featureExtract->get_svm_detector(svm));
    hog.save("/home/tristan/catkin_ws/src/lane_detect/src/svm_detect.mdl");

}
