#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <image_transport/image_transport.h>

#include <cv_bridge/cv_bridge.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define MFORWARD 0
#define MLEFT 1
#define MBACK 2
#define MRIGHT 3
#define MDOWN 4
#define MUP 5
#define MCLOCK 6
#define MCCLOCK 7

#define TAKEOFF "/ardrone/takeoff"
#define LAND "/ardrone/land"
#define RESET "/ardrone/reset"
#define FLATTRIM "/ardrone/flattrim"
#define VEL "cmd_vel"

#define SPEED 0.25
#define SPEED_COEFF 1.0

class Control {
    public:
        explicit Control();
        ~Control();

        void sendCmdT();

    private:
        ros::NodeHandle nh;
        ros::NodeHandle nhPrivate;

        ros::Publisher takeoff_pub;
        ros::Publisher land_pub;
        ros::Publisher reset_pub;
        ros::Publisher flattrim_pub;
        ros::Publisher vel_pub;

        cv::Mat dummy;

        int markerNum;
        int markerAll;

        geometry_msgs::Twist cmdT;

        void clearCmdT();
        void giveCommandToDrone();
        void flySide(int side, double speed);
};
