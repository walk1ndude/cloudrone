// Example of how to use the OpenCV Particle Filter.
//
// Stolen largely from morethantechnical.com's nice mouse_kalman project.
//

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>
#include "marker_detection_node/particleFilter.h"

using namespace std;
using namespace cv;

#define drawCross( center, color, d )                  \
  line( img, cv::Point( center.x - d, center.y - d ),           \
    cv::Point( center.x + d, center.y + d ), color, 2, CV_AA, 0);   \
  line( img, cv::Point( center.x + d, center.y - d ),           \
    cv::Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

Point mpos(-1, -1);
RNG rng;

void on_mouse(int event, int x, int y, int flags, void* param) {
    cout << "Eee" << endl;
    mpos.x = x;
    mpos.y = y;
}

class Particle
{
public:
    Point p;
    float weight;
    Particle(Rect rect)
    {
        p = Point(rng.uniform(rect.x, rect.x + rect.width), rng.uniform(rect.y, rect.y + rect.height));
    }
    void move()
    {
        p += Point(rng.gaussian(5), rng.gaussian(5));
    }
    void measure()
    {
        weight = 1. / norm(p - mpos);
    }
    inline float w() const { return weight; }
};

int _main(int argc, char * const argv[]) {
    rng = theRNG();

    cv::Mat img(650, 650, CV_8UC3);
    char code = (char)-1;

    cv::namedWindow("mouse particle");
    cv::setMouseCallback("mouse particle", on_mouse, 0);

    vector<Particle> pts;
    for (int i = 0; i < 100; i++)
        pts.push_back(Particle(Rect(0, 0, 650, 650)));

    for (;;)
    {
        img = Scalar(128, 128, 128);
        pf::move(pts);
        pf::measure(pts);
        int nmax = 0;
        for (int i = 0; i < pts.size(); i++)
        {
            if (pts[i].w() > pts[nmax].w())
                nmax = i;
            drawCross(pts[i].p, Scalar(0, 255, 0), 2);
        }
        drawCross(pts[nmax].p, Scalar(0, 0, 255), 4);
        drawCross(mpos, Scalar(255, 0, 0), 4);
        imshow("mouse particle", img);
        waitKey(15);
        pf::sample(vector<Particle>(pts), pts);
    }

}
