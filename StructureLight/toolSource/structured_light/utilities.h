/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/7
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <opencv2/opencv.hpp>
#include "virtualCamera.h"
#include <algorithm>
#include <string>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QDir>


class Utilities
{
public:
    Utilities(void);
    ~Utilities(void);
    static bool				XOR(bool val1, bool val2);
    static void				normalize(cv::Vec3f &vec);
    static void				normalize3dtable(double vec[3]);
    static void             pixelToImageSpace(double p[3], cv::Scalar fc, cv::Scalar cc);
    static cv::Point3f      pixelToImageSpace(cv::Point2f p,  VirtualCamera cam);
    static cv::Point2f      undistortPoints( cv::Point2f p,  VirtualCamera cam);
    static cv::Point3f      planeRayInter(cv::Scalar planeNormal,cv::Point3f planePoint, cv::Scalar rayVector, cv::Point3f rayPoint);

    static double			matGet2D(cv::Mat m, int x, int y);
    static double			matGet3D(cv::Mat m, int x, int y, int i);
    static cv::Vec3d		matGet3D(cv::Mat m, int x, int y);
    static void				matSet3D(cv::Mat m, int x, int y, cv::Vec3d);
    static void				matSet3D(cv::Mat m, int x, int y,int i, double val);
    static void				matSet2D(cv::Mat m, int x, int y, double val);

    static void             autoContrast(cv::Mat img_in, cv::Mat &img_out);
    static void				exportMat(const char *path, cv::Mat m);
    static bool             line_lineIntersection(cv::Point3f p1, cv::Vec3f v1, cv::Point3f p2,cv::Vec3f v2,cv::Point3f &p);

    static int				accessMat(cv::Mat m, int x, int y, int i);
    static int				accessMat(cv::Mat m, int x, int y);

    static void             folderScan(const QString dirPath,std::vector<std::string> *filesPath);

};

#endif // UTILITY_H
