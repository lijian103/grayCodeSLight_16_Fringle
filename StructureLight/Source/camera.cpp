#include "Inc/Camera.h"
#include "mainwindow.h"

int  nBuffersInQueue_total = 0;
int  image_id = 0;
String img_name[22]={
    "01.bmp",
    "02.bmp",
    "03.bmp",
    "04.bmp",

    "05.bmp",
    "06.bmp",
    "07.bmp",
    "08.bmp",

    "09.bmp",
    "10.bmp",

    "11.bmp",
    "12.bmp",
    "13.bmp",
    "14.bmp",

    "15.bmp",
    "16.bmp",
    "17.bmp",
    "18.bmp",

    "19.bmp",
    "20.bmp",
    "21.bmp",
    "22.bmp"
                   };
PylonCamera::PylonCamera()
{
   // Create an instant camera object with the camera device found first.
   PylonInitialize();
   // this->ptrCamera =new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());

}

PylonCamera::~PylonCamera()
{
    PylonTerminate();
    //delete  this->ptrCamera;
}


void PylonCamera::initCameraGrap()
{
    // Print the model name of the camera.
    cout << "Using device " << this->ptrCamera->GetDeviceInfo().GetModelName() << endl;
    // The parameter MaxNumBuffer can be used to control the count of buffers
    // allocated for grabbing. The default value of this parameter is 10.
    this->ptrCamera->MaxNumBuffer = 25;

    // Start the grabbing of c_countOfImagesToGrab images.
    // The camera device is parameterized with a default configuration which
    // sets up free-running continuous acquisition.
    this->ptrCamera->StartGrabbing();
    //camera.StartGrabbing(c_countOfImagesToGrab);


}


int PylonCamera::CameraGrap()
{

    try
    {
        grapFlag=1;
        myMutex.lock();
        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
           if(this->ptrCamera->IsGrabbing())
           {

               // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
                this->ptrCamera->RetrieveResult(5000,this->ptrGrabResult, TimeoutHandling_ThrowException);


                // Image grabbed successfully?
                if (this->ptrGrabResult->GrabSucceeded())
                {
                    // Access the image data.;
                    //cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
                    //cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
                    //grapImages=QImage((const unsigned char*)(ptrGrabResult->GetBuffer()),ptrGrabResult->GetWidth(),ptrGrabResult->GetHeight(),QImage::Format_Grayscale8);
                    uint8_t *pImageBuffer = (uint8_t *)ptrGrabResult->GetBuffer();
                    cout << "Gray value of first pixel: " << (uint32_t)pImageBuffer[0]<< endl;

                    // 将 pylon image转成OpenCV image.
                    MainWindow::openCvImage = cv::Mat(this->ptrGrabResult->GetHeight(),this->ptrGrabResult->GetWidth(), CV_8UC1,pImageBuffer);


                    // Display the grabbed image.
                    //新建OpenCV display window.
                    //cv::namedWindow("OpenCV Display Window",cv::WINDOW_AUTOSIZE); // other options: CV_AUTOSIZE, CV_FREERATIO
                    //显示及时影像.
                    //cv::imshow("OpenCV Display Window", MainWindow::openCvImage);
                    //cv::waitKey(20);
                    WaitObject::Sleep(30);



                }
                else
                {
                    cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
                }

            }
           myMutex.unlock();
           grapFlag=0;
    }
    catch (const GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
            << e.GetDescription() << endl;
    }
    return 0;
}

void PylonCamera::initGrab_Strategies()
{
    // Register the standard configuration event handler for enabling software triggering.
    // The software trigger configuration handler replaces the default configuration
    // as all currently registered configuration handlers are removed by setting the registration mode to RegistrationMode_ReplaceAll.
    this->ptrCamera->RegisterConfiguration( new CSoftwareTriggerConfiguration, RegistrationMode_ReplaceAll, Cleanup_Delete);

    // For demonstration purposes only, add sample configuration event handlers to print out information
    // about camera use and image grabbing.
   this->ptrCamera->RegisterConfiguration( new CConfigurationEventPrinter, RegistrationMode_Append, Cleanup_Delete);
   this->ptrCamera->RegisterImageEventHandler( new CImageEventPrinter, RegistrationMode_Append, Cleanup_Delete);

    // Print the model name of the camera.
    cout << "Using device " << this->ptrCamera->GetDeviceInfo().GetModelName() << endl;

    // The MaxNumBuffer parameter can be used to control the count of buffers
    // allocated for grabbing. The default value of this parameter is 10.
    this->ptrCamera->MaxNumBuffer = 15;

    // Open the camera.
    this->ptrCamera->Open();
    if (this->ptrCamera->CanWaitForFrameTriggerReady())
    {
       cout << "Grab using the GrabStrategy_OneByOne default strategy:" << endl << endl;
        this->ptrCamera->StartGrabbing(GrabStrategy_OneByOne);
    }
}


int PylonCamera::Grab_Strategies( )
{
    try
    {
        // Can the camera device be queried whether it is ready to accept the next frame trigger?
        if (this->ptrCamera->CanWaitForFrameTriggerReady())
        {

            // In the background, the grab engine thread retrieves the
            // image data and queues the buffers into the internal output queue.


                // For demonstration purposes, wait for the last image to appear in the output queue.
                WaitObject::Sleep(1000);

                // Check that grab results are waiting.
                if (this->ptrCamera->GetGrabResultWaitObject().Wait(0))
                {
                    cout << endl << "Grab results wait in the output queue." << endl << endl;
                }

                // All triggered images are still waiting in the output queue
                // and are now retrieved.
                // The grabbing continues in the background, e.g. when using hardware trigger mode,
                // as long as the grab engine does not run out of buffers.
                myMutex.lock();
                int nBuffersInQueue = 0;
                while (this->ptrCamera->RetrieveResult(0, ptrGrabResult, TimeoutHandling_Return))
                {
                    nBuffersInQueue++;
                }
                myMutex.unlock();
                cout << "Retrieved " << nBuffersInQueue << " grab results from output queue." << endl << endl;
                nBuffersInQueue_total = nBuffersInQueue_total + nBuffersInQueue;
                if(nBuffersInQueue_total == 22)
                {
                    cout << "成功进行一次图像序列采集，采集图像数为："<< nBuffersInQueue_total<< endl ;
                    nBuffersInQueue_total = 0;
                    image_id = 0;
                }


        }
        else
        {
            // See the documentation of CInstantCamera::CanWaitForFrameTriggerReady() for more information.
            cout << endl;
            cout << "This sample can only be used with cameras that can be queried whether they are ready to accept the next frame trigger.";
            cout << endl;
            cout << endl;
        }
    }
    catch (const GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
    }

    return 0;
}
