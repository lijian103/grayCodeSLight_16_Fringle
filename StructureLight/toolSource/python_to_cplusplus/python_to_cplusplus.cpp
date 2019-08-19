#include "python_to_cplusplus.h"
#include <opencv2/opencv.hpp>
PythonToCplusplus::PythonToCplusplus()
{
    //添加python根目录，不添加会找不到python
    Py_SetPythonHome(L"C:/Users/tommy/Anaconda3");
    Py_Initialize();
    if ( !Py_IsInitialized() )
    {
        std::cout<<"Py_Initialize() failed"<<std::endl;
        exit(-1);
    }
//    PyRun_SimpleString("print('hello python')");

    PyRun_SimpleString("import sys\n");
    PyRun_SimpleString("sys.path.append('./')\n");
    //此句用于matplot显示
    PyRun_SimpleString("sys.argv = ['python.py']");
    PyRun_SimpleString("import cv2");
    PyRun_SimpleString("import matplotlib.pyplot as plt");
    PyRun_SimpleString("import numpy as np");
    PyRun_SimpleString("plt.ion()");    //plt.show() 打开交互模式
    //test matplot
    //PyRun_SimpleString("plt.plot([1, 2, 3, 4], [12, 3, 23, 231])");


}


void PythonToCplusplus::Py_plotShowWave(std::string path,std::string figureName,int col)
{

    PyRun_SimpleString("plt.ion()");
    PyObject* pModule = PyImport_ImportModule("hell");
    if (!pModule) {
            std::cout<<"Cant open python file!\n"<<std::endl;
            exit(-1);
        }
    PyObject* pFunhello= PyObject_GetAttrString(pModule,"testShowImageOneCol");
    if(!pFunhello){
          std::cout<<"Get function hello failed"<<std::endl;
          exit(-1);
      }


        PyObject* args = PyTuple_New(3);   // 2个参数
        PyObject* arg1 = Py_BuildValue("s", path.c_str()); // 字符串参数
        PyObject* arg2 = Py_BuildValue("s", figureName.c_str()); // 字符串参数
        PyObject* arg3 = Py_BuildValue("i", col);
        PyTuple_SetItem(args, 0, arg1);
        PyTuple_SetItem(args, 1, arg2);
        PyTuple_SetItem(args, 2, arg3);
        PyObject_CallObject(pFunhello,args);
        PyRun_SimpleString("plt.ioff()"); /*关闭plot.show()交互模式*/
//        PyRun_SimpleString("plt.show()"); /*调用python文件*/


}




void PythonToCplusplus::Py_plotShowWave(cv::Mat img,std::string figureName,int col)
{
    PyRun_SimpleString("plt.ion()");
    PyObject* pModule = PyImport_ImportModule("hell");
    if (!pModule) {
            std::cout<<"Cant open python file!\n"<<std::endl;
            exit(-1);
        }
    PyObject* pFunhello= PyObject_GetAttrString(pModule,"testTuple");
    if(!pFunhello){
          std::cout<<"Get function hello failed"<<std::endl;
          exit(-1);
      }

    //将Mat转换为一维图片

      cv::Mat tempGrayImg;
      if(img.channels()==3)
      {
        cv::cvtColor(img,tempGrayImg,cv::COLOR_BGR2GRAY);
      }

      cv::Mat tempImg;
      tempGrayImg.convertTo(tempImg, CV_32FC1);

      PyObject *arg_1 = PyTuple_New(img.cols*img.rows+2);

       for( int i = 0; i < img.rows; ++i)
       {
           for( int j = 0; j < img.cols; ++j )
           {
//               std::cout<<(int)tempGrayImg.at<uchar>(i,j);
//               std::cout<<tempImg.at<float>(i,j)<<std::endl;
               PyTuple_SetItem(arg_1, i*img.cols + j, Py_BuildValue("f", tempImg.at<float>(i,j)));
           }
       }

        PyTuple_SetItem(arg_1, img.cols*img.rows, Py_BuildValue("i", img.rows));
        PyTuple_SetItem(arg_1, img.cols*img.rows+1, Py_BuildValue("i",img.cols));

        PyObject* args = PyTuple_New(3);   // 2个参数
        PyTuple_SetItem(args, 0, arg_1);
        PyTuple_SetItem(args, 1, Py_BuildValue("s",figureName.c_str()));
        PyTuple_SetItem(args, 2, Py_BuildValue("i",col));


        PyObject_CallObject(pFunhello,args);
        PyRun_SimpleString("plt.ioff()"); /*关闭plot.show()交互模式,开启阻塞模式*/

//        PyRun_SimpleString("plt.show()"); /*调用python文件*/


}



void PythonToCplusplus::pltShow()
{
  PyRun_SimpleString("plt.ioff()"); /*关闭plot.show()交互模式*/
  PyRun_SimpleString("plt.show()"); /*调用python文件*/

}


void PythonToCplusplus::pyFinalize()
{
    Py_Finalize();
}


 PythonToCplusplus::~PythonToCplusplus()
 {
//    Py_Finalize();
 }


//*********************example**********************
 //    Mat temp=cv::imread(t.imgFilesPath[0].c_str());
 //    Mat temp1=cv::imread(t.imgFilesPath[1].c_str());
 //    PythonToCplusplus mytest;
 //    mytest.Py_plotShowWave("../Pictures/Grap_frame/0_20190704183036.jpg","00",500);
 //    mytest.Py_plotShowWave("../Pictures/Grap_frame/0_20190629143411.jpg","0",500);
 //    mytest.Py_plotShowWave(temp,"1",500);
 //    mytest.Py_plotShowWave(temp1,"11",500);
 //    mytest.pltShow();
 //    mytest.pyFinalize();
