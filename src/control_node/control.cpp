#include "control_node/control.h"
#include <std_msgs/Empty.h>
#include <iostream>

Control::Control() : 
    markerNum(0),
    markerAll(2) {
    
    takeoff_pub = nh.advertise<std_msgs::Empty>(TAKEOFF, 1);
    land_pub = nh.advertise<std_msgs::Empty>(LAND, 1);
    reset_pub = nh.advertise<std_msgs::Empty>(RESET, 1);
    flattrim_pub = nh.advertise<std_msgs::Empty>(FLATTRIM, 1);
    vel_pub = nh.advertise<geometry_msgs::Twist>(VEL, 1);

    while (takeoff_pub.getNumSubscribers() == 0 &&
            land_pub.getNumSubscribers() == 0 && 
            reset_pub.getNumSubscribers() == 0 && 
            vel_pub.getNumSubscribers() == 0) {
        ROS_INFO("connecting");
        sleep(1);
    }

    clearCmdT();
    dummy.create(100, 100, CV_8UC1);
}

Control::~Control() {

}

void Control::clearCmdT() {

    cmdT.linear.x = 0;
    cmdT.linear.y = 0;
    cmdT.linear.z = 0;

    cmdT.angular.x = 0;
    cmdT.angular.y = 0;
    cmdT.angular.z = 0;
}

void Control::sendCmdT() {

    cv::imshow("dummy", dummy);

    clearCmdT();
    giveCommandToDrone();
    
    vel_pub.publish(cmdT);
}

void Control::giveCommandToDrone() {
   
    char c = cv::waitKey(5);
    
    if (c == 'u' || c == 'U') {
        takeoff_pub.publish(std_msgs::Empty());
        clearCmdT();
        ROS_INFO("Takeoff!");
    }
    else if (c == 'x' || c == 'X') {
        land_pub.publish(std_msgs::Empty());
        ROS_INFO("Land!");
    }
    else if (c == 'r' || c == 'R') {
        reset_pub.publish(std_msgs::Empty());
        ROS_INFO("Reset!");
    }
    else if (c == 't' || c == 'T') {
        flattrim_pub.publish(std_msgs::Empty());
        ROS_INFO("Flat trim!");
    }
    else if (c == 'a' || c == 'A') {
        flySide(MLEFT, SPEED);
    }
    else if (c == 'd' || c == 'D') {
        flySide(MRIGHT, SPEED);
    }
    else if (c == 'w' || c == 'W') {
        flySide(MFORWARD, SPEED);
    }
    else if (c == 's' || c == 'S') {
        flySide(MBACK, SPEED);
    }
    else if (c == 'z' || c == 'Z') {
        flySide(MUP, SPEED);
    }
    else if (c == 'c' || c == 'C') {
        flySide(MDOWN, SPEED);
    }
    else if (c == 'q' || c == 'Q') {
        flySide(MCCLOCK, SPEED);
    }
    else if (c == 'e' || c == 'E') {
        flySide(MCLOCK, SPEED);
    }
    else if (c == 'f' || c == 'F') {
        clearCmdT();   
    }
}


void Control::flySide(int side, double speed) {
    
    double * v = NULL;
    double sign;

    switch(side) {
        case MLEFT:
            v = &cmdT.linear.y, sign = 1;
            ROS_INFO("flying left");
            break;
        case MRIGHT:
            v = &cmdT.linear.y, sign = -1;
            ROS_INFO("flying right");
            break;
        case MFORWARD:
            v = &cmdT.linear.x, sign = 1;
            ROS_INFO("flying forward");
            break;
        case MBACK:
            v = &cmdT.linear.x, sign = -1;
            ROS_INFO("flying back");
            break;
        case MUP:
            v = &cmdT.linear.z, sign = 1;
            ROS_INFO("flying up");
            break;
        case MDOWN:
            v = &cmdT.linear.z, sign = -1;
            ROS_INFO("flying down");
            break;
        case MCLOCK:
            v = &cmdT.angular.z, sign = -1;
            ROS_INFO("flying clockwise");
            break;
        case MCCLOCK:
            v = &cmdT.angular.z, sign = 1;
            ROS_INFO("flying counterclockwise");
            break;
    }
    assert(v != NULL);
    speed = std::max(std::min(speed, 1.0), -1.0) * SPEED_COEFF;
    //if (abs(speed) > abs(*v))
    *v = speed * sign;
}
