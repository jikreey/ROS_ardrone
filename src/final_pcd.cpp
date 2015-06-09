#include <ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

void chatterCallback( const sensor_msgs::ImageConstPtr & newimage)
{
    using namespace cv;

    Mat frame, roiImg1, roiImg2, roiImg3;
    int h_min = 0, h_max = 180, s_min = 0, s_max = 255, v_min = 0, v_max = 80; //hitam
    float koreksi = 150;

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    cv_bridge::CvImagePtr bridge;
    bridge = cv_bridge::toCvCopy(newimage, sensor_msgs::image_encodings::BGR8);
    frame = bridge->image;

    Mat roi1 = frame( Rect(0,80,320,40) );
    Mat roi2 = frame( Rect(0,120,320,40) );
    Mat roi3 = frame( Rect(0,160,320,40) );

    Point CoordA;
    Point CoordB;
    Point CoordC;

    cvtColor(roi1,roiImg1,CV_BGR2HSV);
    cvtColor(roi2,roiImg2,CV_BGR2HSV);
    cvtColor(roi3,roiImg3,CV_BGR2HSV);
    inRange(roiImg1,Scalar(h_min,s_min,v_min),Scalar(h_max,s_max,v_max),roiImg1);
    inRange(roiImg2,Scalar(h_min,s_min,v_min),Scalar(h_max,s_max,v_max),roiImg2);
    inRange(roiImg3,Scalar(h_min,s_min,v_min),Scalar(h_max,s_max,v_max),roiImg3);

    //line( frame, Point( 0, 60 ), Point( 320, 60), Scalar( 255, 0, 0),  2, 8 );
    /*line( frame, Point( 0, 60 ), Point( 64, 60), Scalar( 200, 100, 0),  2, 8 );
    line( frame, Point( 65, 60 ), Point( 128, 60), Scalar( 200, 200, 0),  2, 8 );
    line( frame, Point( 129, 60 ), Point( 192, 60), Scalar( 200, 0, 0),  2, 8 );
    line( frame, Point( 193, 60 ), Point( 256, 60), Scalar( 200, 200, 0),  2, 8 );
    line( frame, Point( 257, 60 ), Point( 320, 60), Scalar( 200, 100, 0),  2, 8 );*/

    erode(roiImg1, roiImg1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(roiImg1, roiImg1, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(roiImg2, roiImg2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(roiImg2, roiImg2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(roiImg3, roiImg3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate(roiImg3, roiImg3, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

    findContours(roiImg2.clone(), contours, hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));
    for (unsigned int j = 0; j < contours.size(); j++) {
            float areaKalibrasi = contourArea(contours[j]);
            if (areaKalibrasi > 2000 && areaKalibrasi < 3000) {
                Moments mm = moments(contours[j], false);
                double moment10 = mm.m10;
                double moment01 = mm.m01;
                double moment00 = mm.m00;
                //Point CoordB;
                CoordB.x = int(moment10 / moment00);
                CoordB.y = int(moment01 / moment00);
                Point2f center(CoordB.x, CoordB.y+140);  // point in center (x only)
                Point2f text(5,15);
                circle(frame, center, 5, Scalar(0, 0, 255), -1, 8, 0);
                circle(frame, center, 15.0, Scalar( 0, 255, 0 ), 1, 8 );
                char TestStr[100];
                char Areas[100];
                sprintf(Areas,"Area 2 = %.f Coord = %d", areaKalibrasi, CoordB.x );
                putText(frame,Areas,Point(5,45), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),1);
                if(CoordB.x < 64) {
                    sprintf(TestStr,"Rotasi Kiri");
                    putText(frame,TestStr,text, FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),2);
                } else if (CoordB.x > 64 && CoordB.x <128) {
                    sprintf(TestStr,"Geser Kiri");
                    putText(frame,TestStr,text, FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),2);
                } else if (CoordB.x > 128 && CoordB.x <192) {
                    sprintf(TestStr,"Maju");
                    putText(frame,TestStr,text, FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),2);
                } else if (CoordB.x > 192 && CoordB.x <256) {
                    sprintf(TestStr,"Geser Kanan");
                    putText(frame,TestStr,text, FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),2);
                } else if (CoordB.x > 256 && CoordB.x <320) {
                    sprintf(TestStr,"Rotasi Kanan");
                    putText(frame,TestStr,text, FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),2);
                }

            findContours(roiImg1.clone(), contours, hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));
            for (unsigned int i = 0; i < contours.size(); i++) {
                    float areaA = contourArea(contours[i]);
                    if ((areaKalibrasi-koreksi) < areaA && areaA < (areaKalibrasi+koreksi)) {
                        Moments mm = moments(contours[i], false);
                        double moment10 = mm.m10;
                        double moment01 = mm.m01;
                        double moment00 = mm.m00;
                        //Point CoordA;
                        CoordA.x = int(moment10 / moment00);
                        CoordA.y = int(moment01 / moment00);
                        Point2f center(CoordA.x, CoordA.y+100); // point in center (x only)
                        circle(frame, center, 5, Scalar(0, 0, 255), -1, 8, 0);
                        circle(frame, center, 15.0, Scalar( 0, 255, 0 ), 1, 8 );
                        char Areas[100];
                        sprintf(Areas,"Area 1 = %.f Coord = %d", areaA, CoordA.x );
                        putText(frame,Areas,Point(5,30), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),1);
                    } else if ((areaKalibrasi-koreksi) > areaA || areaA > (areaKalibrasi+koreksi)) {
                        char Areas[100];
                        sprintf(Areas,"Bukan garis");
                        putText(frame,Areas,Point(5,30), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),1);
                    }
                 }

            findContours(roiImg3.clone(), contours, hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));
            for (unsigned int k = 0; k < contours.size(); k++) {
                    float areaC = contourArea(contours[k]);
                    if ((areaKalibrasi-koreksi) < areaC && areaC < (areaKalibrasi+koreksi)) {
                        Moments mm = moments(contours[k], false);
                        double moment10 = mm.m10;
                        double moment01 = mm.m01;
                        double moment00 = mm.m00;
                        //Point CoordC;
                        CoordC.x = int(moment10 / moment00);
                        CoordC.y = int(moment01 / moment00);
                        Point2f center(CoordC.x, CoordC.y+180);  // point in center (x only)
                        circle(frame, center, 5, Scalar(0, 0, 255), -1, 8, 0);
                        circle(frame, center, 15.0, Scalar( 0, 255, 0 ), 1, 8 );
                        char Areas[100];
                        sprintf(Areas,"Area 3 = %.f Coord = %d", areaC, CoordC.x );
                        putText(frame,Areas,Point(5,60), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),1);
                    } else if ((areaKalibrasi-koreksi) > areaC || areaC > (areaKalibrasi+koreksi)) {
                        char Areas[100];
                        sprintf(Areas,"Bukan garis");
                        putText(frame,Areas,Point(5,60), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),1);
                    }
                 }

            } else if (areaKalibrasi < 2000 || areaKalibrasi > 3000) {
                char Areas[100];
                sprintf(Areas,"GARIS TAK TERDETEKSI");
                putText(frame,Areas,Point(5,30), FONT_HERSHEY_PLAIN, 1, Scalar(0,0,255),2);
            }
         }


    line( frame, Point( CoordA.x,CoordA.y+100 ), Point( CoordB.x,CoordB.y+140), Scalar( 255, 0, 0),  1, 8 );
    line( frame, Point( CoordB.x,CoordB.y+140 ), Point( CoordC.x,CoordC.y+180), Scalar( 255, 0, 0),  1, 8 );

    imshow("camera",frame);
    imshow("frame 1",roiImg1);

    int pressedkey;
    pressedkey = waitKey(1);

}

int main( int argc, char **argv)
{
    ros::init(argc, argv, "terbang");
    ros::NodeHandle skripsi;
    ros::Rate loop_rate(10);

    image_transport::ImageTransport it(skripsi);
    image_transport::Subscriber it_sub = it.subscribe("ardrone/front/image_raw", 1, chatterCallback);


    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
