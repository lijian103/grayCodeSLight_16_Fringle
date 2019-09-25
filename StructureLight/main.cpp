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

    //**********test******************
    Mat I = imread( samples::findFile("../Pictures/Hardware_trigger_frame/13.bmp"), IMREAD_GRAYSCALE);
        if( I.empty()){
            cout << "Error opening image" << endl;
            return EXIT_FAILURE;
        }
        Mat padded;                            //expand input image to optimal size
        int m = getOptimalDFTSize( I.rows );
        int n = getOptimalDFTSize( I.cols ); // on the border add zero values
        copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, BORDER_CONSTANT, Scalar::all(0));
        Mat planes[] = {Mat_<float>(padded), Mat::zeros(padded.size(), CV_32F)};
        Mat complexI;
        merge(planes, 2, complexI);         // Add to the expanded another plane with zeros
        dft(complexI, complexI);            // this way the result may fit in the source matrix
        // compute the magnitude and switch to logarithmic scale
        // => log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
        split(complexI, planes);                   // planes[0] = Re(DFT(I), planes[1] = Im(DFT(I))
        magnitude(planes[0], planes[1], planes[0]);// planes[0] = magnitude
        Mat magI = planes[0];
        magI += Scalar::all(1);                    // switch to logarithmic scale
        log(magI, magI);
        // crop the spectrum, if it has an odd number of rows or columns
        magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));
        // rearrange the quadrants of Fourier image  so that the origin is at the image center
        int cx = magI.cols/2;
        int cy = magI.rows/2;
        Mat q0(magI, Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
        Mat q1(magI, Rect(cx, 0, cx, cy));  // Top-Right
        Mat q2(magI, Rect(0, cy, cx, cy));  // Bottom-Left
        Mat q3(magI, Rect(cx, cy, cx, cy)); // Bottom-Right
        Mat tmp;                           // swap quadrants (Top-Left with Bottom-Right)
        q0.copyTo(tmp);
        q3.copyTo(q0);
        tmp.copyTo(q3);
        q1.copyTo(tmp);                    // swap quadrant (Top-Right with Bottom-Left)
        q2.copyTo(q1);
        tmp.copyTo(q2);
        std::cout<<"magI.type()"<<magI<<"\t"<<magI.depth()<<std::endl;
        normalize(magI, magI, 0, 1, NORM_MINMAX); // Transform the matrix with float values into a
                                                // viewable image form (float between values 0 and 1).
         std::cout<<"magI.type()"<<magI<<"\t"<<magI.depth()<<std::endl;
        imshow("Input Image"       , I   );    // Show the result
        imshow("spectrum magnitude", magI);
    //**************test*****************

    thread thread_soft_1(high_soft_entry,false);
    thread_soft_1.detach();
    thread thread_soft_2(low_soft_entry,false);
    thread_soft_2.detach();
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
