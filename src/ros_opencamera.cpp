#include <ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Empty.h>
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>


using namespace cv;
using namespace std;

std_msgs::Empty emp_msg;


void chatterCallback( const sensor_msgs::ImageConstPtr & newimage)
{
    Mat frame;

    cv_bridge::CvImagePtr bridge;
    bridge = cv_bridge::toCvCopy(newimage, sensor_msgs::image_encodings::BGR8);
    frame = bridge->image;

    imshow("camera",frame);

    int pressedkey;
    pressedkey = waitKey(1);

}

int main( int argc, char **argv)
{
    ros::init(argc, argv, "terbang");
    ros::NodeHandle camera;
    ros::NodeHandle node;
    ros::Rate loop_rate(10);
    ros::Publisher pub_toggle_cam;

    image_transport::ImageTransport it(camera);
    image_transport::Subscriber it_sub = it.subscribe("ardrone/front/image_raw", 1, chatterCallback);

    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
