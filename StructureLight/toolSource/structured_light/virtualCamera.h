/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/7
 */

#ifndef VIRTUALCAMERA_H
#define VIRTUALCAMERA_H

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

class VirtualCamera
{

public:

	VirtualCamera(void);
	~VirtualCamera(void);

    void loadCalibrationError(std::string path);
	void loadDistortion(std::string path);
	void loadCameraMatrix(std::string path);
	void loadRotationMatrix(std::string path);
	void loadTranslationVector(std::string path);
	void computeProjectionMatrix();
    void computeHomographyMatrix();

    cv::Mat calibrationError;

    cv::Mat cameraMatrix;
	cv::Mat distortion;
	cv::Mat rotationMatrix;
	cv::Mat translationVector;

	cv::Mat projectionMatrix;
    cv::Mat homographyMatrix;

	cv::Point3f position;

	cv::Point2f fc; 
	cv::Point2f cc; 

	int width;
	int height;

private:

	int loadMatrix(cv::Mat &matrix,int s1,int s2 ,std::string file);

};

#endif // VIRTUALCAMERA_H
