#ifndef PYTHON_TO_CPLUSPLUS_H
#define PYTHON_TO_CPLUSPLUS_H
#include "Python.h"
#include "iostream"
#include <opencv2/opencv.hpp>

class PythonToCplusplus
{
public:
    PythonToCplusplus();
    ~PythonToCplusplus();
    void Py_plotShowWave(std::string path,std::string figureName,int col);
    void Py_plotShowWave(cv::Mat img,std::string figureName="test",int col=505);
    void pltShow();
    void pyFinalize();
};

#endif // PYTHON_TO_CPLUSPLUS_H
