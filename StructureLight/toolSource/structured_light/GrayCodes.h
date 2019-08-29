/**@file time_measurement.h
 * @brief contains definitions for the GRAY_CODES Class
 * @copyright lijian
 * data 2019/8/23
 */

#ifndef GRAY_CODES_H_
#define GRAY_CODES_H_


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "utilities.h"

#include <iostream>
#include <fstream>

#include <math.h>


#define GRAY_MAX_NUM 100

class GrayCodes	{
	public: 
		///Constructor
        GrayCodes(int projW, int projH,  std::string path);

		///Destructor
		~GrayCodes();

		void unload();
		int getNumOfImgs();
        cv::Mat* getNextImg();
		
        cv::Mat* getImg(int num);

		void generateGrays();

		void save();
        static int grayToDec(std::vector<bool> gray);
        int getNumOfRowBits();
        int getNumOfColBits();
		
	protected:
        cv::Mat* grayCodes[GRAY_MAX_NUM];
        cv::Mat* colorCodes[GRAY_MAX_NUM];

		void calNumOfImgs();

		void allocMemForImgs();

		bool imgsLoaded;

		int numOfImgs;
		int numOfRowImgs;
		int numOfColImgs;

		int currentImgNum;
		
		int height;
		int width;
        std::string path;
};

#endif
