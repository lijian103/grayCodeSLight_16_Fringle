/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/7
 */

#ifndef PROJECTOR_CALIBRATION_H
#define PROJECTOR_CALIBRATION_H


#include "camera_calibration.h"
#include "utilities.h"

class ProjectorCalibration: public CameraCalibration
{
public:    
    ProjectorCalibration();

    void setProjectorCornersSize(cv::Size size);
    cv::Size getProjectorCornersSize();

    void loadProjectorImgs(const QString ImgPath);
    cv::Mat findCameraHomography_imgTo3D(std::vector<cv::Point3f> &objPoints3D, std::vector<cv::Point2f> &imgPoints);
    void findCameraAllHomography_imgTo3D();

    void reSetProjectParm();
    bool findCornersInCamImg_projector(cv::Mat img,std::vector<cv::Point2f> *camCorners,int threshold_value);
    bool extractImageCorners_projector();
    bool findProjectorExtrisics(const QString  projectorFilesDir,const QString  cameraFilesDir,std::string  exPath);

    std::vector <cv::Mat> homography;
    int numOfCornersX_projector=7;
    int numOfCornersY_projector=9;
    cv::Mat projectorImg;

    std::string projectorImgPath;


private:

};

#endif // PROJECTOR_CALIBRATION_H
