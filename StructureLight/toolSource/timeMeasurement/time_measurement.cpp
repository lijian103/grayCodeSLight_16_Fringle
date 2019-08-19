/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/2
 */

#include <opencv2/core/utility.hpp>
#include <iostream>
#include <iomanip>
#include "time_measurement.h"

/** @brief  Contains all mytool classes, functions, etc. */
namespace  mytool
{
    TimeMeasurement::TimeMeasurement()
    {
        this->tick_freq=cv::getTickFrequency();
        this->start_time=(double)cv::getTickCount();
    }

    TimeMeasurement::~TimeMeasurement()
    {

    }

    void TimeMeasurement::UpdateStartTime()
    {
        this->start_time=(double)cv::getTickCount();

    }

    double TimeMeasurement::GetTimeConsumption()
    {
        double time_consumption=((double)cv::getTickCount()-this->start_time)/this->tick_freq;
        return time_consumption;
    }

    void TimeMeasurement::PrintTimeConsumption()
    {
        double time_consumption=((double)cv::getTickCount()-this->start_time)/this->tick_freq;
        std::cout<<"consumption time :"<<std::fixed<<std::setprecision(10)<<time_consumption<<std::endl;
    }


}

//*****************example*****************************************
//    Mat image;
//    Mat gray_image;
//    image = imread( "../Pictures/Hardware_trigger_frame/Htf_7_9/cos_wave_Period_59_initial_phase_0.bmp", IMREAD_COLOR );
//    cvtColor(image, gray_image, COLOR_BGR2GRAY);
//    mytool::TimeMeasurement t1;
//    for(int i=0;i<gray_image.rows;i++)
//    {
//        for(int j = 0; j < gray_image.cols; j++)
//        {
//            gray_image.at<uchar>(i,j) = j % 256;
//            if (gray_image.at<uchar>(i,j) > 88)
//                gray_image.at<uchar>(i,j)=j%256;
//            else
//                gray_image.at<uchar>(i,j) = j /256;
//        }
//    }
//    t1.PrintTimeConsumption();


