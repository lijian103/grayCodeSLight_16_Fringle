/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/27
 */

#ifndef RECONSTRUCTOR_H
#define RECONSTRUCTOR_H

#include "VirtualCamera.h"
#include "PointCloudImage.h"
#include "GrayCodes.h"

class Reconstructor
{
public:
    Reconstructor();
    ~Reconstructor(void);
    void loadCameras(VirtualCamera	*camera,std::string camFolder);

    void loadCamImgs(VirtualCamera *camera,std::string folder,std::string prefix,std::string suffix);
    void unloadCamImgs();

    void setBlackThreshold(int val);
    void setWhiteThreshold(int val);

    void computeShadows(VirtualCamera &camera);
    void saveShadowImg(const char path[]);

    bool getProjPixelPattrnIndex(int x, int y, int &grayPattenIndex);
    void decodePaterns(VirtualCamera &camera, std::vector<std::vector<cv::Point>> &Pixels);

    void cam2WorldSpace(VirtualCamera cam, cv::Point3f &p);
    void triangulation(std::vector<std::vector<cv::Point>> &cam1Pixels, VirtualCamera camera1, std::vector<std::vector<cv::Point>> &cam2Pixels, VirtualCamera camera2);
    void runReconstruction();

    VirtualCamera cam;
    VirtualCamera projector;


    int blackThreshold;
    int whiteThreshold;

    std::vector<cv::Mat> camImgs;
    cv::Mat colorImg;

//    cv::Mat color;
    cv::Mat shadowMask;

    std::vector<std::vector<cv::Point>> camPixels; //general functions use this instead of cam1Pixels or cam2Pixels
    std::vector<std::vector<cv::Point>> projectorPixels; //general functions use this instead of cam1Pixels or cam2Pixels


    PointCloudImage *points3DProjView;

    int numberOfPattrns;
    int numOfColBits;
    int numOfRowBits;

    bool autoContrast;
    bool saveAutoContrast;
};

#endif // RECONSTRUCTOR_H
