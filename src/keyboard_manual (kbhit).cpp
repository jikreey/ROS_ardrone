/*
Dzikri Purnama
Elektronika dan Instrumentasi - Universitas Gadjah Mada
Ardrone manual flying using keyboard.
How to use:
1. Run the program.
2. Take off first.
3. Control it using "a,s,d,w,q,e,i,k".
4. Landing it, and the program will stop immadiately.
*/

#include <ros/ros.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <termios.h>
#include <iostream>

using namespace std;

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


//fungsi capture keyboard
char m ='x';
void ambilchar()
{
    init_keyboard();
    if(kbhit())m=readch();
}

int main(int argc, char** argv)
{

    ros::init(argc, argv,"ARDrone keyboard manual");
    ros::NodeHandle node;
    ros::Rate loop_rate(50);

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
                geserka.linear.y=-0.1;
                geserka.linear.z=0.0;
                geserka.angular.x=0.0;
                geserka.angular.y=0.0;
                geserka.angular.z=0.0;

    //geser kiri message
                geserki.linear.x=0.0;
                geserki.linear.y=+0.1;
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
                rotasika.angular.z=-0.1;

    //rotasi kiri message
                rotasiki.linear.x=0.0;
                rotasiki.linear.y=0.0;
                rotasiki.linear.z=0.0;
                rotasiki.angular.x=0.0;
                rotasiki.angular.y=0.0;
                rotasiki.angular.z=+0.1;

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
        <<"e = rotasi kanan      r = reset\n"
        <<"\n"
        <<"LET'S FLY!\n";


while (ros::ok()) {

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
            m='x';
            break;

        case 'l':
            pub_twist.publish(hover); //drone is flat
            pub_empty_land.publish(msg); //lands the drone
            ROS_INFO("Landing");
            exit(0);
            m = 'x';
            break;

        case 'h':
            ROS_INFO("hover");
            pub_twist.publish(hover);
            m = 'x';
            break;

        case 'w':
            ROS_INFO("gerak maju");
            pub_twist.publish(maju);
            m='x';
            break;

        case 's':
            ROS_INFO("gerak mundur");
            pub_twist.publish(mundur);
            m = 'x';
            break;

        case 'd':
            ROS_INFO("gerak kanan");
            pub_twist.publish(geserka);
            m = 'x';
            break;

        case 'a':
            ROS_INFO("gerak kiri");
            pub_twist.publish(geserki);
            m = 'x';
            break;

        case 'e':
            ROS_INFO("rotasi ke kanan");
            pub_twist.publish(rotasika);
            m = 'x';
            break;

        case 'q':
            ROS_INFO("rotasi ke kiri");
            pub_twist.publish(rotasiki);
            m = 'x';
            break;

        case 'i':
            ROS_INFO("naik");
            pub_twist.publish(naik);
            m = 'x';
            break;

        case 'k':
            ROS_INFO("turun");
            pub_twist.publish(turun);
            m = 'x';
            break;
        }
    }
    ros::spinOnce();
    loop_rate.sleep();

}//ros::ok

}//main
