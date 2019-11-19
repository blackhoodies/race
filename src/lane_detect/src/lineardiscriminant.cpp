#include "lineardiscriminant.h"

LinearDiscriminant::LinearDiscriminant() {}
LinearDiscriminant::~LinearDiscriminant() {}

void LinearDiscriminant::load_images( const String & dirname, vector< Mat > & img_lst)
{
    vector< String > files;
    glob( dirname, files );
    for ( size_t i = 0; i < files.size(); ++i )
    {
        Mat img = imread( files[i] ); // load the image
        if ( img.empty() )            // invalid image, skip it.
        {
            //cout << files[i] << " is invalid!" << endl;
            continue;
        }
        
        img_lst.push_back( img );
    }
}
/*
void sample_neg( const vector< Mat > & full_neg_lst, vector< Mat > & neg_lst, const Size & size )
{
    Rect box;
    box.width = size.width;
    box.height = size.height;
    const int size_x = box.width;
    const int size_y = box.height;
    srand( (unsigned int)time( NULL ) );
    for ( size_t i = 0; i < full_neg_lst.size(); i++ )
        if ( full_neg_lst[i].cols > box.width && full_neg_lst[i].rows > box.height )
        {
            box.x = rand() % ( full_neg_lst[i].cols - size_x );
            box.y = rand() % ( full_neg_lst[i].rows - size_y );
            Mat roi = full_neg_lst[i]( box );
            neg_lst.push_back( roi.clone() );
        }
}
*/

void LinearDiscriminant::computeHOGs( const Size wsize, const vector< Mat > & img_lst, vector< Mat > & gradient_lst, bool use_flip )
{
    HOGDescriptor hog;
    hog.winSize = wsize;
    Mat gray;
    vector< float > descriptors;
    for( size_t i = 0 ; i < img_lst.size(); i++ )
    {
        if ( img_lst[i].cols >= wsize.width && img_lst[i].rows >= wsize.height )
        {
            Rect r = Rect(( img_lst[i].cols - wsize.width ) / 2,
                          ( img_lst[i].rows - wsize.height ) / 2,
                          wsize.width,
                          wsize.height);
            cvtColor( img_lst[i](r), gray, COLOR_BGR2GRAY );
            hog.compute( gray, descriptors, Size( 8, 8 ), Size( 0, 0 ) );
            gradient_lst.push_back( Mat( descriptors ).clone() );
            if ( use_flip )
            {
                flip( gray, gray, 1 );
                hog.compute( gray, descriptors, Size( 8, 8 ), Size( 0, 0 ) );
                gradient_lst.push_back( Mat( descriptors ).clone() );
            }
        }
    }
}

void LinearDiscriminant::train(const Mat &trainData, const Mat &trainLabels)
{
    set<int> classes;
    for (size_t i=0; i<trainLabels.total(); ++i)
        classes.insert(trainLabels.at<int>(i));
    int C = classes.size(); // unique labels
    int N = trainData.rows;
    int num_components = (C-1); // to keep for LDA

    PCA pca(trainData, Mat(), cv::PCA::DATA_AS_ROW, (N-C));
    mean = pca.mean.reshape(1,1);
    LDA lda(trainData, trainLabels, num_components);

    Mat leigen;
    lda.eigenvectors().convertTo(leigen, pca.eigenvectors.type());
    gemm(pca.eigenvectors, leigen, 1.0, Mat(), 0.0, eigenvectors, GEMM_1_T);

    Mat features; 
    for (int i=0; i<trainData.rows; i++)
    {
        // here's the actual magic. we don't use the lda's eigenvecs,
        // but the *product* of pca and lda eigenvecs to do the projection:
        Mat proj = LDA::subspaceProject(eigenvectors, mean, trainData.row(i));
        features.push_back( proj );
    }
    labels = trainLabels;
 }

int LinearDiscriminant::predict(Mat &feature)
{
        Mat proj = LDA::subspaceProject(eigenvectors, mean, feature);
        // compare to pre-projected train feature.row(i),
        // return id of item with shortest distance
}