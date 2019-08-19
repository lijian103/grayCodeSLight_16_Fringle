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

// ����OpenCV API
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

    MainWindow w;
    w.show();
    return a.exec();
}



//opencv���
//cv::Mat c1=(cv::Mat_<double>(1,3) <<2, 0,0);
//cv::Mat c2=(cv::Mat_<double>(1,3) <<0, 2,0);
//Mat c3 = c1.cross(c2);
//std::cout<<c3<<std::endl;


//��⣺
//����ͶӰ���Ǹ�������������ȷ�����������ϵ�еĵ����꣬����ͶӰ�ߣ���Ȼ����ȷ���������,��ȷ������ͶӰ�ߡ�
//ȷ��z=1���Եõ�����ͶӰ�������һ��z=1�ĵ㣬�����������ϵ�У�������������Եõ���������ϵ�е�����λ��
//��ϸ��Ϣ�鿴����cv::Point3f Utilities::pixelToImageSpace(cv::Point2f p,  VirtualCamera cam)


