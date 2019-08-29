// Contains an Image Event Handler that prints a message for each event method call.

#ifndef INCLUDED_IMAGEEVENTPRINTER_H_7884943
#define INCLUDED_IMAGEEVENTPRINTER_H_7884943

#include <pylon/ImageEventHandler.h>
#include <pylon/GrabResultPtr.h>
#include <iostream>

// 加载OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
using namespace cv;
extern int image_id;
extern String img_name[22];

// Namespace for using cout.
using namespace std;
namespace Pylon
{
    class CInstantCamera;

    class CImageEventPrinter : public CImageEventHandler
    {
    public:

        virtual void OnImagesSkipped( CInstantCamera& camera, size_t countOfSkippedImages)
        {
            std::cout << "OnImagesSkipped event for device " << camera.GetDeviceInfo().GetModelName() << std::endl;
            std::cout << countOfSkippedImages  << " images have been skipped." << std::endl;
            std::cout << std::endl;
        }


        virtual void OnImageGrabbed( CInstantCamera& camera, const CGrabResultPtr& ptrGrabResult)
        {
            std::cout << "OnImageGrabbed event for device " << camera.GetDeviceInfo().GetModelName() << std::endl;	
            // Image grabbed successfully?
			//新建一个OpenCV image对象.
			Mat openCvImage;

            if (ptrGrabResult->GrabSucceeded())
            {
				std::cout << "image_id: " << image_id << std::endl;
				/*std::cout << "SizeX: " << ptrGrabResult->GetWidth() << std::endl;
                std::cout << "SizeY: " << ptrGrabResult->GetHeight() << std::endl;
				std::cout << std::endl;*/
                uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
				openCvImage = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC1, pImageBuffer);
                String image_name = "../Pictures/Hardware_trigger_frame/"+img_name[image_id];
                cv::imwrite(image_name, openCvImage);
				image_id++;
//                if (image_id == 10)
//				{
//					image_id = 0;
//				}

				//// Display the grabbed image.
				////新建OpenCV display window.
				//namedWindow("OpenCV Display Window", cv::WINDOW_AUTOSIZE); // other options: CV_AUTOSIZE, CV_FREERATIO
				////显示及时影像.
				//imshow("OpenCV Display Window", openCvImage);
                //waitKey(20);
            }
            else
            {
                std::cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << std::endl;
            }
        }
    };
}

#endif /* INCLUDED_IMAGEEVENTPRINTER_H_7884943 */
