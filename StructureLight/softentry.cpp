#include <Windows.h>
#include <iostream>
#include "softEntry.h"


#include <stdio.h>
#include <thread>
#include <mutex>
#include <Windows.h>
#include "toolSource/timeMeasurement/time_measurement.h"
#include "toolSource/structured_light/camera_calibration.h"
#include "toolSource/structured_light/utilities.h"
#include "toolSource/python_to_cplusplus/python_to_cplusplus.h"
#include "toolSource/structured_light/GrayCodes.h"

using namespace std;

class ThreadTest
{
public:
    //卖票线程1
    void ThreadTest::Thread1();
    //卖票线程2
    void ThreadTest::Thread2();


    ThreadTest();
    ~ThreadTest();

private:
    //票的剩余数目
    int Sum;

    mutex Mutex;//线程锁

};




void ThreadTest::Thread1(){

    for (;;){
        Mutex.lock();//加锁
        Sleep(10);
        --Sum;

        if (Sum < 0){
            printf("Thrad1——票卖完了\n", Sum);
            break;
        }
        printf("Thrad1——剩余票数：%d\n", Sum);
        Mutex.unlock();//解锁

    }

    Mutex.unlock();//解锁

}


void  ThreadTest::Thread2(){

    for (;;){

        Mutex.lock();//加锁
        Sleep(10);
        --Sum;

        if (Sum < 0){
            printf("Thrad2——票卖完了\n");
            break;
        }
        printf("Thrad2——剩余票数：%d\n", Sum);

        Mutex.unlock();//解锁
    }

    Mutex.unlock();//解锁

}

//构造函数
ThreadTest::ThreadTest()
{
    Sum = 50;
    thread t1(&ThreadTest::Thread1,this);
    t1.detach();
    thread t2(&ThreadTest::Thread2,this);
    t2.detach();


}
//析构函数
ThreadTest::~ThreadTest()
{

}








void realtime_soft_entry(bool loop)
{
    while(loop)
    {
         Sleep(30);
    }
}


void high_soft_entry(bool loop)
{

//   GrayCodes test(912,1140,"D:/Trunk/Pictures/test/");
//   test.generateGrays();
//   test.save();




    my_main();
    cv::Mat M(2,2,CV_8UC3,cv::Scalar::all(0));

    std::cout<<"M="<<""<<M<<endl<<endl;

    std::vector<cv::Point2f> vPoints(20);
   for (size_t i = 0; i < vPoints.size(); ++i)
       vPoints[i] = cv::Point2f((float)(i * 5), (float)(i % 7));
   cout << "A vector of 2D Points = " << cv::Mat(vPoints) << endl << endl;

   cv::Point2f tt(2,4);
   std::cout<<tt<<std::endl;

    while(loop)
    {
        std::cout<<"high_soft_entry"<<std::endl;
        Sleep(200);
    }
}

void low_soft_entry(bool loop)
{
    //***********



    cv::Mat t1(2,3,CV_8U,cv::Scalar(5));
    cv::Mat t2(2,3,CV_8U,cv::Scalar(255));
    std::cout<<t1+t2<<std::endl;

    uchar t3=6;
    uchar t4=255;
    uchar t5=cv::saturate_cast<uchar>(t3+t4);
    std::cout<<"t3+t4"<<(int)t5<<"t3+t4"<<std::endl;


    //***************
    while(loop)
    {
        std::cout<<"low_soft_entry"<<std::endl;
        Sleep(1000);
    }
}

void my_main()
{

    cv::Vec3f c1(2, 0,0);
    cv::Vec3f c2(0, 2,0);
    cv::Vec3f c3= c1.cross(c2);
    std::cout<<c3<<std::endl;

    cv::Mat imgtemp=cv::imread("../Pictures/Grap_frame/20190821141647_3.bmp") ;
    cv::Scalar intensity = imgtemp.at<cv::Vec3b>(cv::Point(4, 5));
    int blue = intensity.val[0];
    int green = intensity.val[1];
    int red = intensity.val[2];
    std::cout<<intensity<<std::endl<<blue<<std::endl<<green<<std::endl<<red<<std::endl;


    cv::Mat img_grey;
    cv::Mat img_grey_all;

    cv::cvtColor(imgtemp, img_grey, cv::COLOR_BGR2GRAY );
    std::cout<<img_grey.channels()<<std::endl<<std::endl;
    std::cout<<img_grey.channels()<<std::endl<<std::endl;

    int threshold_type = 0;
    int const max_binary_value = 255;

        cv::threshold(img_grey, img_grey_all,66, max_binary_value, threshold_type );
    //    adaptiveThreshold(img_grey, img_grey_auto, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 31, 10);//自适应阈值分割
        cv::imwrite("../Pictures/Grap_frame/20190821093809_02.bmp",img_grey_all);
    //    cv::imwrite("../Pictures/Grap_frame/20190821093809_03.bmp",img_grey_auto);



        PythonToCplusplus mytest;
        mytest.Py_plotShowWave("../Pictures/Hardware_trigger_frame/21.bmp","00",824);
        mytest.pltShow();
        mytest.pyFinalize();

//        float t1=2.0;
//        float t2=4;
//        std::cout<<t1/t2<<std::endl;
}

