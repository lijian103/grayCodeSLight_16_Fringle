#include "mainwindow.h"
#include <QApplication>
#include <Windows.h>
#include <iostream>
#include "Inc/Camera.h"
#include "softEntry.h"
#include "toolSource/timeMeasurement/time_measurement.h"
#include "toolSource/structured_light/camera_calibration.h"
#include "toolSource/structured_light/utilities.h"
#include "toolSource/python_to_cplusplus/python_to_cplusplus.h"

// 加载OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>

using namespace Pylon;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    thread thread_soft_1(high_soft_entry,false);
    thread_soft_1.detach();
    thread thread_soft_2(low_soft_entry,false);
    thread_soft_2.detach();



//    Mat imgtemp=cv::imread("../Pictures/Grap_frame/20190821141647_3.bmp") ;
//    Mat img_grey;
//    Mat img_grey_all;

//    cv::cvtColor(imgtemp, img_grey, cv::COLOR_BGR2GRAY );

//    int threshold_type = 0;
//    int const max_binary_value = 255;

//    cv::threshold(img_grey, img_grey_all,66, max_binary_value, threshold_type );
////    adaptiveThreshold(img_grey, img_grey_auto, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 31, 10);//自适应阈值分割
//    cv::imwrite("../Pictures/Grap_frame/20190821093809_02.bmp",img_grey_all);
////    cv::imwrite("../Pictures/Grap_frame/20190821093809_03.bmp",img_grey_auto);

//    PythonToCplusplus mytest;
//    mytest.Py_plotShowWave("../Pictures/Grap_frame/20190821093809_03.bmp","00",781);
//    mytest.pltShow();
//    mytest.pyFinalize();

    MainWindow w;
    w.show();
    return a.exec();
}



//opencv叉乘，求平面度的法向量
//cv::Mat c1=(cv::Mat_<double>(1,3) <<2, 0,0);
//cv::Mat c2=(cv::Mat_<double>(1,3) <<0, 2,0);
//Mat c3 = c1.cross(c2);
//std::cout<<c3<<std::endl;


//理解：
//逆向投影线是根据收缩因子来确定在相机坐标系中的点坐标，逆向投影线，仍然是先确定点坐标的,再确定逆向投影线。
//确定z=1可以得到逆向投影线上面的一个z=1的点，是在相机坐标系中，跟外参运算后可以得到世界坐标系中的坐标位置
//详细信息查看函数cv::Point3f Utilities::pixelToImageSpace(cv::Point2f p,  VirtualCamera cam)

//处理格点图
//Mat imgtemp=cv::imread("../Pictures/Grap_frame/grid/camera_calibration_board.bmp") ;
//imgtemp=imgtemp*1.5;//1.5产生的图片效果较好
//cv::imwrite("../Pictures/Grap_frame/grid/camera_calibration_board_2_1.5.bmp",imgtemp);
