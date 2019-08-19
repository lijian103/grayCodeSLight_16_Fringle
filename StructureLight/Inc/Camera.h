#ifndef CAMERA_H_
#define CAMERA_H_

#include <QObject>
#include <mutex>
#include <thread>
// º”‘ÿOpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>


// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>
//#ifdef PYLON_WIN_BUILD
//#    include <pylon/PylonGUI.h>
//#endif
// Include files to use the pylon API.
#include <pylon/PylonIncludes.h>
// Include files used by samples.
#include "include/ConfigurationEventPrinter.h"
#include "include/ImageEventPrinter.h"

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using cout.
using namespace std;

// Namespace for using opencv.
using namespace cv;
extern String img_name[8];
extern int  nBuffersInQueue_total;
extern int  image_id ;
namespace Ui {
class PylonCamera;
}

class PylonCamera
{
public:
    explicit PylonCamera();
    ~PylonCamera();
    CInstantCamera *ptrCamera;
    int CameraGrap();
    void initCameraGrap();
    int Grab_Strategies();
    void initGrab_Strategies();
     // This smart pointer will receive the grab result data.
    CGrabResultPtr ptrGrabResult;

private:




private slots:

};

#endif //CAMERA_H_
