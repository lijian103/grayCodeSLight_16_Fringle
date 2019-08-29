/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/22
 */
#ifndef POINT_CLOUD_IMAGE_
#define POINT_CLOUD_IMAGE_

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "utilities.h"

#include <iostream>
#include <fstream>

#define MAXSIZE (1600*1200)

class PointCloudImage
{
	public:

		PointCloudImage(int imageW,int imageH, bool color);
		~PointCloudImage(void);

		bool setPoint(int i_w, int j_h, cv::Point3f point, cv::Vec3f colorBGR);
		bool setPoint(int i_w, int j_h, cv::Point3f point);

		bool getPoint(int i_w, int j_h, cv::Point3f &pointOut);
		bool getPoint(int i_w, int j_h, cv::Point3f &pointOut, cv::Vec3f &colorOut);

		bool addPoint(int i_w, int j_h, cv::Point3f point, cv::Vec3f color);
		bool addPoint(int i_w, int j_h, cv::Point3f point);

		bool hasColor();

        void exportNumOfPointsPerPixelImg(const char *path);
        void exportXYZ(const char *path,bool exportOffPixels=true, bool colorFlag=true);

		int getWidth();
		int getHeight();

	private:
		
		int w;
		int h;
		bool colorFlag;

		cv::Mat points;
		cv::Mat numOfPointsForPixel;
		cv::Mat color;
};

#endif //_MESH_CREATOR_
