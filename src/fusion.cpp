/*
Dzikri Purnama
Elektronika dan Instrumentasi - Universitas Gadjah Mada
Keyboard dengan getche.
*/

#include <ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <termios.h>
#include <cv.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

    geometry_msgs::Twist twist_msg;
    geometry_msgs::Twist hover;
    geometry_msgs::Twist maju;
    geometry_msgs::Twist mundur;
    geometry_msgs::Twist rotasika;
    geometry_msgs::Twist rotasiki;
    geometry_msgs::Twist geserka;
    geometry_msgs::Twist geserki;
    geometry_msgs::Twist naik;
    geometry_msgs::Twist turun;
    std_msgs::Empty msg;

static struct termios initial_settings, new_settings;
static int peek_character = -1;
void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}

void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}

int kbhit()
{
    char ch;
    int nread;

    if(peek_character != -1)
        return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);

    if(nread == 1) {
        peek_character = ch;
        return 1;
    }
    return 0;
}

int readch()
{
    char ch;

    if(peek_character != -1) {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

char getch()
{
    char buf=0;
    struct termios old ={0};
    if(tcgetattr(0,&old)<0)perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN]=1;
    old.c_cc[VTIME]=0;
    if(tcsetattr(0,TCSANOW,&old)<0)perror("tcsetattr ICANON");
    if(read(0,&buf,1)<0)perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if(tcsetattr(0,TCSADRAIN,&old)<0)perror("tcsetattr ~ICANON");
    return (buf);
}

int getche(void) {
    struct termios oldattr, newattr;
    int ch=0;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

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

//fungsi capture keyboard
char m =' ';
void ambilchar()
{
init_keyboard();
if(kbhit())m=readch();
//else if(!kbhit())m='x';
//close_keyboard();
}

int main(int argc, char** argv)
{

    printf("Manual Test Node Starting");
    ros::init(argc, argv,"ARDrone_manual_test");
    ros::NodeHandle node;
    ros::NodeHandle camera;
    ros::Rate loop_rate(50);
    image_transport::ImageTransport it(camera);
    image_transport::Subscriber it_sub = it.subscribe("ardrone/image_raw", 1, chatterCallback);

    ros::Publisher pub_empty_takeoff;
    ros::Publisher pub_empty_land;
    ros::Publisher pub_empty_reset;
    ros::Publisher pub_twist;
/*
    -linear.x: move backward
    +linear.x: move forward
    -linear.y: move right
    +linear.y: move left
    -linear.z: move down
    +linear.z: move up
    -angular.z: turn left
    +angular.z: turn right
*/

    //hover message
                hover.linear.x=0.0;
                hover.linear.y=0.0;
                hover.linear.z=0.0;
                hover.angular.x=0.0;
                hover.angular.y=0.0;
                hover.angular.z=0.0;

    //maju message
                maju.linear.x=0.1;
                maju.linear.y=0.0;
                maju.linear.z=0.0;
                maju.angular.x=0.0;
                maju.angular.y=0.0;
                maju.angular.z=0.0;

    //mundur message
                mundur.linear.x=-0.1;
                mundur.linear.y=0.0;
                mundur.linear.z=0.0;
                mundur.angular.x=0.0;
                mundur.angular.y=0.0;
                mundur.angular.z=0.0;

    //geser kanan message
                geserka.linear.x=0.0;
                geserka.linear.y=-0.2;
                geserka.linear.z=0.0;
                geserka.angular.x=0.0;
                geserka.angular.y=0.0;
                geserka.angular.z=0.0;

    //geser kiri message
                geserki.linear.x=0.0;
                geserki.linear.y=+0.2;
                geserki.linear.z=0.0;
                geserki.angular.x=0.0;
                geserki.angular.y=0.0;
                geserki.angular.z=0.0;


    //rotasi kanan message
                rotasika.linear.x=0.0;
                rotasika.linear.y=0.0;
                rotasika.linear.z=0.0;
                rotasika.angular.x=0.0;
                rotasika.angular.y=0.0;
                rotasika.angular.z=-0.2;

    //rotasi kiri message
                rotasiki.linear.x=0.0;
                rotasiki.linear.y=0.0;
                rotasiki.linear.z=0.0;
                rotasiki.angular.x=0.0;
                rotasiki.angular.y=0.0;
                rotasiki.angular.z=+0.2;

    //naik message
                naik.linear.x=0.0;
                naik.linear.y=0.0;
                naik.linear.z=+0.1;
                naik.angular.x=0.0;
                naik.angular.y=0.0;
                naik.angular.z=0.0;

    //turun message
                turun.linear.x=0.0;
                turun.linear.y=0.0;
                turun.linear.z=-0.1;
                turun.angular.x=0.0;
                turun.angular.y=0.0;
                turun.angular.z=0.0;

        pub_twist         = node.advertise<geometry_msgs::Twist>("/cmd_vel", 1);       /* Message queue length is just 1 */
        pub_empty_takeoff = node.advertise<std_msgs::Empty>("/ardrone/takeoff", 1);    /* Message queue length is just 1 */
        pub_empty_land    = node.advertise<std_msgs::Empty>("/ardrone/land", 1);       /* Message queue length is just 1 */
        pub_empty_reset   = node.advertise<std_msgs::Empty>("/ardrone/reset", 1);      /* Message queue length is just 1 */


    ROS_INFO("Starting ARdrone_test loop");
    cout<<"Map keyboard:\n"
        <<"a = geser kiri        i = naik\n"
        <<"s = mundur            k = turun\n"
        <<"d = geser kanan       j = takeoff\n"
        <<"w = maju              l = landing\n"
        <<"q = rotasi kiri       h = hover\n"
        <<"e = rotasi kanan      r = reset\n";

char m=' ';
while (ros::ok())
{
    init_keyboard();
    if(kbhit())
    {
        m=readch();
        switch(m)
        {
        case 'j':
            pub_empty_takeoff.publish(msg); //launches the drone
            pub_twist.publish(hover); //drone is flat
            ROS_INFO("Taking off");
            ros::spinOnce();
            loop_rate.sleep();
            m=' ';
            break;

        case 'l':
            pub_twist.publish(hover); //drone is flat
            pub_empty_land.publish(msg); //lands the drone
            ROS_INFO("Landing");
            exit(0);
            m = ' ';
            break;

        case 'h':
            ROS_INFO("hover");
            pub_twist.publish(hover);
            m = ' ';
            break;

        case 'w':
            ROS_INFO("gerak maju");
            pub_twist.publish(maju);
            m=' ';
            break;

        case 's':
            ROS_INFO("gerak mundur");
            pub_twist.publish(mundur);
            m = ' ';
            break;

        case 'd':
            ROS_INFO("gerak kanan");
            pub_twist.publish(geserka);
            m = ' ';
            break;

        case 'a':
            ROS_INFO("gerak kiri");
            pub_twist.publish(geserki);
            m = ' ';
            break;

        case 'e':
            ROS_INFO("rotasi ke kanan");
            pub_twist.publish(rotasika);
            m = ' ';
            break;

        case 'q':
            ROS_INFO("rotasi ke kiri");
            pub_twist.publish(rotasiki);
            m = ' ';
            break;

        case 'i':
            ROS_INFO("naik");
            pub_twist.publish(naik);
            m = ' ';
            break;

        case 'k':
            ROS_INFO("turun");
            pub_twist.publish(turun);
            m = ' ';
            break;
        }
    }
    ros::spinOnce();
    loop_rate.sleep();

}//ros::ok

}//main
