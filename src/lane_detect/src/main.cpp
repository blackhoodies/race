#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

#include <opencv2/highgui/highgui.hpp>

#include "detectlane.h"
#include "carcontrol.h"
#include "featureextract.h"

#include <string>

bool STREAM = true;

VideoCapture capture("video.avi");
DetectLane *detect;
CarControl *car;
FeatureExtract* featureExtract;
int skipFrame = 1;
int c = 0;
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{

    cv_bridge::CvImagePtr cv_ptr;
    Mat out;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        cv::imshow("View", cv_ptr->image);
	    waitKey(1);
        
        int windows_n_rows = 20;
        int windows_n_cols = 20;
        int StepSlide = 20;
        /*
        for (int row = 0; row <= cv_ptr->image.rows - windows_n_rows; row += StepSlide)
        {
            for (int col = 0; col <= cv_ptr->image.cols - windows_n_cols; col += StepSlide)
            {
                Rect windows(col, row, windows_n_rows, windows_n_cols);
                Mat Roi = cv_ptr->image(windows);
                char name[10];
                sprintf(name, "%d.png", c);
                cv::imwrite(name, Roi);
                c++;
            }
        }
        */
       

    
        
        
        detect->update(cv_ptr->image);
        car->driverCar(detect->getLeftLane(), detect->getRightLane(), 50);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

void depthImageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        cv::imshow("Depth", cv_ptr->image);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

/*
void videoProcess()
{
    Mat src;
    while (true)
    {
        capture >> src;
        if (src.empty()) break;
        
        imshow("View", src);
        detect->update(src);
        waitKey(30);
    }
}

*/
int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    cv::namedWindow("View");
    cv::namedWindow("Binary");
    cv::namedWindow("Threshold");
    cv::namedWindow("Bird View");
    cv::namedWindow("Lane Detect");
    //cv::namedWindow("Depth");
    
    detect = new DetectLane();
    car = new CarControl();

    if (STREAM) {
        // cv::startWindowThread();

        ros::NodeHandle nh;
        image_transport::ImageTransport it(nh);
        image_transport::Subscriber sub = it.subscribe("team1/camera/rgb", 1, imageCallback);

        //ros::NodeHandle nh2;
        //image_transport::ImageTransport it2(nh2);
        //image_transport::Subscriber sub2 = it2.subscribe("team1/camera/depth", 1, depthImageCallback);

        ros::spin();
    } 
/*
else {
        videoProcess();
    }
*/
    cv::destroyAllWindows();
}
