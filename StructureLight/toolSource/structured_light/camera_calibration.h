/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/19
 */
#ifndef CAMERA_CALIBRATION_H
#define CAMERA_CALIBRATION_H

#include <opencv2/opencv.hpp>
#include <string>
#include <QString>
#include <QFileInfo>
#include <QDebug>
#include <QDir>

class CameraCalibration
{
public:

    #define CAMCALIB_OUT_MATRIX 1
    #define CAMCALIB_OUT_DISTORTION 2
    #define CAMCALIB_OUT_ROTATION 3
    #define CAMCALIB_OUT_TRANSLATION 4
    #define CAMCALIB_OUT_ERROR 5

    CameraCalibration();
    ~CameraCalibration(void);

    //set SquareSize data
    void	setSquareSize(cv::Size2d size);
    cv::Size getSquareSize();
    void setCornersSize(cv::Size size);
    cv::Size getCornersSize();

    void	exportTxtFiles(const char *pathNfileName, int CAMCALIB_OUT_PARAM);

    void getImgFilesPath(const QString dirPath);
    void loadCameraImgs(const QString dirPath);
    void unloadCameraImgs();
    void loadCameraExtriImgs(const QString extrinImgPath);
    void unloadCameraExtriImg();


    bool findCornersInCamImg(cv::Mat img,std::vector<cv::Point2f> *camCorners);
    bool findCornersInObjetBoard(std::vector<cv::Point3f> *objCorners);
    bool extractImageCorners();


    bool calibrateCamera();
    bool findCameraExtrisics();
    bool undistortionImg(const QString distortionImgPath);
    void undistortCameraImgPoints(std::vector<cv::Point2f> points_in,std::vector<cv::Point2f> &points_out);

    void perspectiveTransformation(std::vector<cv::Point2f> corners_in,cv::Mat homoMatrix, std::vector<cv::Point3f> &points_out);

    void loadCalibData(const char *path);
    void saveCalibData(const char *path);
    static void loadCameraConfig(const char *path,std::vector<std::string> &cameraParam);
    static void saveCameraConfig(const char *path,std::vector<std::string> &cameraParam);


    std::vector<cv::Point2f> manualMarkCheckBoard(cv::Mat img);
    float markWhite(cv::Mat img);

    cv::Mat camMatrix = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0));//内外参矩阵，H——单应性矩阵;
    cv::Mat distortion = cv::Mat(1, 5, CV_32FC1, cv::Scalar::all(0));//摄像机的5个畸变系数：k1,k2,p1,p2,k3;
    cv::Mat rotationVector;//每幅图像的旋转向量（罗德里格旋转向量）
    cv::Mat rotationMatrix;//每幅图像的旋转矩阵
    cv::Mat translationVector;//每幅图像的平移向量，t



    //images
    std::vector <cv::Mat> calibImgs;
    cv::Mat               extrImg;

    std::vector<std::string> imgFilesPath;


    //评价标定结果
    std::vector <double> err_of_each_calibImgs;//单张图像的误差
    double               total_err = 0;//所有图像的平均误差
    double               err_of_extrImg = 0;//对外参图像的误差
    bool                 camCalibrated;

    //角点数
    int numOfCornersX=6;
    int numOfCornersY=4;
    cv::Size2d squareSize=cv::Size(20,20);//参考单位为mm，所有的保持一致
    cv::Size camImageSize;

    std::vector<std::vector<cv::Point2f>> imgBoardCornersCam;
    std::vector<std::vector<cv::Point3f>> objBoardCornersCam;

private:

};

#endif // CAMERA_CALIBRATION_H
