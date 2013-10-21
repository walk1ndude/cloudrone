#ifndef MARKER_DETECTION_NODE_H
#define MARKER_DETECTION_NODE_H

#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <ardrone_autonomy/Navdata.h>
#include <cloudrone/Object.h>
#include <deque>

using namespace std;
using namespace cv;

/// =========================== Definitions ===========================\

#define OBJECT_THUMB_SIZE Size(30, 30)
#define OBJECT_THUMB_DIR_PATH "/home/ardrone/objects"


#define GRAD (CV_PI/180.)

#define CAMERA_PARAMS_FILE "cameraParams.yml"
#define CAM_HORIZON_CORR (6*GRAD) // 4.5
#define GYRO_X_CORRECT (-1*GRAD) // flat trim must fix it
#define VIEW_M2P 550.0 // 600
#define VIEW_P2M (1./VIEW_M2P)
#define VIEW_SCALE 0.1
#define VIDEO2NAVDATA_DELAY 0.07 // sec, relative to navdata

/// =========================== ObjectDetector ===============================

class ObjectDetector
{
public:
    explicit ObjectDetector();

private:

    ros::NodeHandle handle;
    ros::Subscriber navdata_sub;
    ros::Publisher objects_pub;

    image_transport::ImageTransport it;
    image_transport::Subscriber camera_image;
    cv_bridge::CvImagePtr cv_ptr;

    // frame
    Mat K, D;
    Mat frame;
    void loadCameraCalibration(const string & filename);

    // birds eye
    Size view_size;
    Mat view;
    Mat view_H;
    Mat getBirdsEyeTransform(double yaw, double pitch, double roll, double altd) const;
    void tryImageIMSync();

    // marker
    vector<pair<CascadeClassifier, int> > cascadeDetectors;
    void findObjects();
    cloudrone::Object predictObjectPos(const cloudrone::Object & object, const ardrone_autonomy::Navdata & navdata_prev, const ardrone_autonomy::Navdata & navdata);
    int viewid;
    vector<cloudrone::Object> objects;

    // navdata
    deque<ardrone_autonomy::Navdata> navdataQueue;
    deque<cv_bridge::CvImagePtr> imageQueue;
    ardrone_autonomy::Navdata navdata, navdata_prev;
    ardrone_autonomy::Navdata navdataInterpolate(const ardrone_autonomy::Navdata & nd0, ardrone_autonomy::Navdata & nd1, ros::Time & middle_stamp);

    // events
    void onImage(const sensor_msgs::ImageConstPtr & msg);
    void onNavdata(const ardrone_autonomy::Navdata & navdata);
};


#endif
