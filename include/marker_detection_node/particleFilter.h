#pragma once

#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

namespace pf
{

template<typename PT> double sum_w(const vector<PT> & pts) 
{
	double sw = 0.0;
	for (int i = 0; i < pts.size(); i++)
		sw += pts[i].w();
	return sw;
}

template<typename PT> void move(vector<PT> & pts)
{
	for (int i = 0; i < pts.size(); i++)
		pts[i].move();
}
template<typename PT> void measure(vector<PT> & pts)
{
	for (int i = 0; i < pts.size(); i++)
		pts[i].measure();
}
template<typename PT> void sample(const vector<PT> & pts, vector<PT> & dst_pts)
{		
	RNG rng = theRNG();
	vector<float> diffs(dst_pts.size() + 1);
	double sum_diffs = 0.0;
	for (int i = 0; i < dst_pts.size() + 1; i++)
	{
		diffs[i] = -log(1.0 - rng.uniform(0.f, 1.f));
		sum_diffs += diffs[i];
	}
	float scale = sum_w(pts) / sum_diffs;
	int index = 0;
	float beta = 0.f;
	for (int i = 0; i < dst_pts.size(); i++)
	{
		beta += diffs[i] * scale;
		while (beta > pts[index].w())
		{
			beta -= pts[index].w();
			index++; // or %
		}
		dst_pts[i] = pts[index];
	}
}

template<typename PT> double efficiency(const vector<PT> & pts)
{
	if (pts.size() > 0)
	{
		double s1 = 1.0 / sum_w(pts);
		double N = 0;
		for (int i = 0; i < pts.size(); i++)
		{
			double w = pts[i].w() * s1;
			N += w*w;
		}
		double k1 = 1.0/pts.size();
		return ((k1 / N) - k1) / (1-k1);
	}
	else
		return 0;
}

template<typename PT> vector<PT> peaks(const vector<PT> & pts, float nbh_radius, float w_thresh)
{
	vector<bool> F(pts.size(), true);
	vector<PT> centers;
	for (int i = 0; i < pts.size(); i++)
	{
		cout << pts[i].w() << endl;
		if (F[i] && pts[i].w() >= w_thresh)
		{
			for (int j = 0; j < pts.size(); j++)
			{
				if (i != j && pts[i].dist(pts[j]) <= nbh_radius)
				{
					if (pts[i].w() < pts[j].w())
					{
						F[i] = false;
						break;
					}
					else
					{
						F[j] = false;
					}
				}
			}
			if (F[i])
			{
				centers.push_back(pts[i]);
			}
		}
	}
	return centers;
}

}
