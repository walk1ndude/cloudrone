#include "marker_detection_node/marker_detection_node.h"

ObjectDetector::ObjectDetector() : it(handle), _handle("~")
{
    camera_image = it.subscribe("ardrone/image_raw", 1, &ObjectDetector::onImage, this);
    navdata_sub = handle.subscribe("ardrone/navdata", 1, &ObjectDetector::onNavdata, this);
    objects_pub = handle.advertise<cloudrone::Object>("cloudrone/objects", 1);
    
    _handle.param("calibdata", calibdata, std::string(CAMERA_PARAMS_FILE));

    loadCameraCalibration(calibdata);

    cascadeDetectors.resize(2);
    cascadeDetectors[0].first.load("/home/ardrone/cascadeTreeLBP.xml");
    cascadeDetectors[0].second = 0;
    
    cascadeDetectors[1].first.load("/home/ardrone/cascadeStarNew.xml");
    cascadeDetectors[1].second =1;

    view_size = Size(500, 500); // TEMP
    viewid = 0;
}

void ObjectDetector::loadCameraCalibration(const string & filename)
{
    FileStorage camera_params_file(filename, FileStorage::READ);
    assert(camera_params_file.isOpened());
    camera_params_file["cameraMatrixFront"] >> K;
    camera_params_file["distCoeffsFront"] >> D;
    assert(!K.empty() && !D.empty());
}

/// ========================== Events ====================================
void ObjectDetector::onNavdata(const ardrone_autonomy::Navdata & navdata)
{
  if (!navdataQueue.empty() && navdata.header.stamp < navdataQueue.back().header.stamp)
    {
      imageQueue.clear();
      navdataQueue.clear();
    }
    navdataQueue.push_back(navdata);
    tryImageIMSync();
}

void ObjectDetector::onImage(const sensor_msgs::ImageConstPtr & msg)
{
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    cv_ptr->header.stamp = cv_ptr->header.stamp - ros::Duration(VIDEO2NAVDATA_DELAY);
    if (!imageQueue.empty() && cv_ptr->header.stamp < imageQueue.back()->header.stamp)
    {
      imageQueue.clear();
      navdataQueue.clear();
    }
    imageQueue.push_back(cv_ptr);
    tryImageIMSync();
}

void ObjectDetector::tryImageIMSync()
{
  //  std::cout<<"imageQueue.size "<<imageQueue.size()<<std::endl;
   // std::cout<<"navdataQueue.size "<<navdataQueue.size()<<std::endl;
    
    //for (int i = 0; i < imageQueue.size(); i++) cout << imageQueue[i]->header.stamp << ", ";
    //cout << " | ";
    //for (int i = 0; i < navdataQueue.size(); i++) cout << navdataQueue[i].header.stamp << ", ";
    //cout << endl;
    while(imageQueue.size() > 0)
    {
        cv_bridge::CvImagePtr cv_ptr = imageQueue.front();
        while (1 < navdataQueue.size() && navdataQueue[1].header.stamp < cv_ptr->header.stamp) navdataQueue.pop_front();

        if (1 < navdataQueue.size())
        {
            if (cv_ptr->header.stamp >= navdataQueue[0].header.stamp)
            {
                undistort(cv_ptr->image, frame, K, D);

                navdata_prev = navdata;
                ardrone_autonomy::Navdata & nd0 = navdataQueue[0], nd1 = navdataQueue[1];
                navdata = navdataInterpolate(nd0, nd1, cv_ptr->header.stamp);

                navdata.altd = 800;
                if (navdata.altd > 0)
                {
                    view_H = getBirdsEyeTransform(navdata.rotZ*GRAD, navdata.rotY*GRAD, navdata.rotX*GRAD, navdata.altd*0.001);
                    warpPerspective(frame, view, view_H, view_size, INTER_CUBIC | WARP_INVERSE_MAP);
                    imshow("view", view);
                    findObjects();
                }
                imshow("frame", frame);
            }
            imageQueue.pop_front();
        } else
            break;
    }
   waitKey(1);
}

/// ==========================================================

ardrone_autonomy::Navdata ObjectDetector::navdataInterpolate(const ardrone_autonomy::Navdata & nd0, ardrone_autonomy::Navdata & nd1, ros::Time & middle_stamp)
{
    double  d0 = 1-(middle_stamp.toSec() - nd0.header.stamp.toSec()) / (nd1.header.stamp.toSec() - nd0.header.stamp.toSec()),
            d1 = 1-(nd1.header.stamp.toSec() - middle_stamp.toSec()) / (nd1.header.stamp.toSec() - nd0.header.stamp.toSec());

    ardrone_autonomy::Navdata _navdata;
    #define navdata_mean(var) _navdata.var = (nd0.var*d0 + nd1.var*d1)
    navdata_mean(rotX);
    navdata_mean(rotY);
    navdata_mean(rotZ);
    navdata_mean(vx);
    navdata_mean(vy);
    navdata_mean(vz);
    navdata_mean(ax);
    navdata_mean(ay);
    navdata_mean(az);
    navdata_mean(altd);
    navdata_mean(pressure);
    #undef navdata_mean
    return _navdata;
}

Mat ObjectDetector::getBirdsEyeTransform(double yaw, double pitch, double roll, double altd) const
{
    assert(!K.empty());

    double alpha = pitch - CV_PI/2 - CAM_HORIZON_CORR;
    double beta = -(roll + GYRO_X_CORRECT);
    double gamma = 0;

    double fx = K.at<double>(0, 0);
    double fy = K.at<double>(1, 1);
    double cx = K.at<double>(0, 2);
    double cy = K.at<double>(1, 2);

    double dist = altd * fy * VIEW_SCALE; // 0.8 for screenshot

    // Projection 2D -> 3D matrix
    Mat A1 = (Mat_<double>(4,3) <<
        1, 0, -view_size.width*0.5, // -cx
        0, 1, -view_size.height, // -6404
        0, 0,   0,
        0, 0,   1); // dist2

    // Rotation matrices around the X,Y,Z axis
    Mat RX = (Mat_<double>(4, 4) <<
        1,          0,           0, 0,
        0, cos(alpha), -sin(alpha), 0,
        0, sin(alpha),  cos(alpha), 0,
        0,          0,           0, 1);

    Mat RY = (Mat_<double>(4, 4) <<
        cos(beta), 0, -sin(beta), 0,
                0, 1,          0, 0,
        sin(beta), 0,  cos(beta), 0,
                0, 0,          0, 1);

    Mat RZ = (Mat_<double>(4, 4) <<
        cos(gamma), -sin(gamma), 0, 0,
        sin(gamma),  cos(gamma), 0, 0,
        0,          0,           1, 0,
        0,          0,           0, 1);

    // Composed rotation matrix with (RX,RY,RZ)
    Mat R = RX * RY * RZ;

    // Translation matrix on the Z axis change dist will change the height
    Mat T = (Mat_<double>(4, 4) <<
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, dist,
        0, 0, 0, 1);

    // Camera Intrisecs matrix 3D -> 2D
    Mat A2 = (Mat_<double>(3,4) <<
        fx, 0, cx, 0,
        0, fy, cy, 0,
        0, 0,   1, 0);

    // Finally
    Mat H = A2 * (R * (T * A1)); // T * R maybe
    return H;
}

inline Point2f RemapPoint(const Mat & H, Point2f pos)
{
#define _H ((const double*)H.data)
    float x = _H[0] * pos.x + _H[1] * pos.y + _H[2];
    float y = _H[3] * pos.x + _H[4] * pos.y + _H[5];
    float z = _H[6] * pos.x + _H[7] * pos.y + _H[8];
    return Point2f(x / z, y / z);
#undef _H
}

cloudrone::Object ObjectDetector::predictObjectPos(const cloudrone::Object & object, const ardrone_autonomy::Navdata & navdata_prev, const ardrone_autonomy::Navdata & navdata)
{

}

static int framenumber=0;

void ObjectDetector::findObjects()
{
    Mat demo = view.clone();
    /*for (int i = 0; i < cascadeDetectors.size(); i++)
    {
        vector<Rect> found;
        cascadeDetectors[i].first.detectMultiScale(view, found);
        for (int r = 0; r < found.size(); r++)
        {
            Mat thumb;
            resize(Mat(view, found[r]), thumb, OBJECT_THUMB_SIZE);
            char buf[256];
            sprintf(buf, "%s/%d.png", OBJECT_THUMB_DIR_PATH, viewid);
            imwrite(buf, thumb);

            cloudrone::Object object;

            object.viewid = viewid++;
            object.classid = cascadeDetectors[i].second;
            object.y = (view.cols/2 - (found[r].x+found[r].width*0.5)) * (VIEW_P2M / VIEW_SCALE);
            object.x = (view.rows - (found[r].y+found[r].height*0.5)) * (VIEW_P2M / VIEW_SCALE);
            object.z = 0;
            object.wx = found[r].width * (VIEW_P2M / VIEW_SCALE);
            object.wy = found[r].height * (VIEW_P2M / VIEW_SCALE);
            object.wz = 0;

            rectangle(demo, found[r], Scalar(0,0,255));
            objects_pub.publish(object);
        }
    }*/
    

    
    ROS_DEBUG_STREAM("Some output based on the computation");
    
    if(framenumber==20){
    
      std::cout<<"framenumber==20"<<std::endl;
    cloudrone::Object object;

    object.viewid = 0;
    object.classid = 1;
    
    objects_pub.publish(object);
    }
    
    if(framenumber==40){
    
       std::cout<<"framenumber==40"<<std::endl;
    cloudrone::Object object;

    object.viewid = 1;
    object.classid = 2;

    
    objects_pub.publish(object);
    }
    framenumber++;
    
    imshow("demo", demo);
    waitKey(1);
    
}

/// ============== MAIN ======================
int main(int argc, char ** argv)
{
    ros::init(argc, argv, "find_marker_node");

    ObjectDetector N;

    ros::Rate loop_rate(50);
    while (ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
