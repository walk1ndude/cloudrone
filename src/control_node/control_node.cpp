#include <ros/ros.h>

#include "control_node/control.h"

#define NODE_RATE 30

int main(int argc, char ** argv) {
    
    ros::init(argc, argv, "control_node");

    Control * control = new Control;
    
    ros::Rate rosRate(NODE_RATE);
    
    while (ros::ok()) {
        ros::spinOnce();
        control->sendCmdT();
        rosRate.sleep();
    }

    delete control;

    return 0;
}
