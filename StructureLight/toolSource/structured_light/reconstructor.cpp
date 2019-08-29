/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/27
 */

#include "reconstructor.h"
#include "utilities.h"
#include <Windows.h>

Reconstructor::Reconstructor()
{
    autoContrast=true;
    saveAutoContrast=true;
    numberOfPattrns=22;
    numOfColBits=10;
    //BlackThreshold仅仅与计算shadowMask有关，取值待定
    setBlackThreshold(8);
    setWhiteThreshold(1);



//    this->camPixels.resize(projector.width);
//    this->projectorPixels.resize(projector.width);
    points3DProjView = nullptr;

}

Reconstructor::~Reconstructor(void)
{
    unloadCamImgs();

    if(points3DProjView)
        delete points3DProjView ;
}

void Reconstructor::loadCameras(VirtualCamera *camera,std::string camFolder)
{
    std::string path;

    path = camFolder;
    path += "output/cam_matrix.txt";
    camera->loadCameraMatrix(path.c_str());

    path = camFolder;
    path += "output/cam_distortion.txt";
    camera->loadDistortion(path.c_str());

    path = camFolder;
    path += "output/cam_rotation_matrix.txt";
    camera->loadRotationMatrix(path.c_str());

    path = camFolder;
    path += "output/cam_trans_vectror.txt";
    camera->loadTranslationVector(path.c_str());

    camera->height=0;
    camera->width =0;
}


//load camera images
void Reconstructor::loadCamImgs(VirtualCamera *camera, std::string folder,std::string prefix,std::string suffix)
{

    cv::Mat tmp;

    std::cout<<"Loading Camera Images...\n";

    if(!camImgs.empty())
        unloadCamImgs();


    if(saveAutoContrast&&autoContrast)
    {
        std::stringstream p;
        p<<folder.c_str() << "AutoContrastSave/";

        QDir *temp = new QDir;
        bool exist = temp->exists(QString::fromStdString(p.str()));
        if(exist)
            std::cout<<p.str()<<"文件夹已经存在"<<std::endl;
        else
        {
            bool ok = temp->mkdir(QString::fromStdString(p.str()));
            if( ok )
                 std::cout<<p.str()<<"文件夹创建成功"<<std::endl;
        }
    }

    for(int i=0; i< numberOfPattrns;i++)
    {
        std::stringstream path;

        path<<folder.c_str();
        path << prefix.c_str();
        if(i+1<10)
            path<<"0";
        path << i+1 << suffix.c_str();

        tmp.release();

        //std::cout<<path.str().c_str()<<std::endl;

        tmp = cv::imread(path.str().c_str());

        if(tmp.empty())
        {
            std::cout<<"\nError loading cam image "<<i+1<<".........\n";
            exit(-1);
        }

        //auto contrast
        if(autoContrast)
        {
            Utilities::autoContrast(tmp,tmp);

            if(saveAutoContrast)
            {
                std::stringstream temp;
                temp<<folder.c_str() << "AutoContrastSave/"<<"AutoContrastSave"<< prefix.c_str() ;
                if(i+1<10)
                    temp<<"0";
                temp<< i+1 << suffix.c_str();
                cv::imwrite(temp.str().c_str(),tmp);
            }
        }

        if(i==0)
        {
            colorImg = tmp.clone();
        }
        cv::cvtColor(tmp, tmp,  cv::COLOR_BGR2GRAY);

        camImgs.push_back(tmp);
    }

    if(camera->width==0)
    {
        camera->height=camImgs[0].rows;
        camera->width =camImgs[0].cols;
    }

    std::cout<<"loadCamImgs done!\n";

}



//unload camera images
void Reconstructor::unloadCamImgs()
{

    if(camImgs.size())
    {
        for(int i=0; i<camImgs.size(); i++)
        {
            camImgs[i].release();
        }
    }

    colorImg.release();

    camImgs.clear();
}

void Reconstructor::setBlackThreshold(int val)
{
    blackThreshold = val;
}

void Reconstructor::setWhiteThreshold(int val)
{
    whiteThreshold = val;
}


void Reconstructor::computeShadows(VirtualCamera &camera)
{
    std::cout<<"Estimating Shadows...\n";

    int w = camera.width;
    int h = camera.height;

    shadowMask.release();

    shadowMask = cv::Mat(h,w,CV_8U,cv::Scalar(0));

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {
            float blackVal, whiteVal;

            blackVal  = (float) Utilities::matGet2D( camImgs[1], i, j);
            whiteVal  = (float) Utilities::matGet2D( camImgs[0], i, j);

            if(whiteVal - blackVal > blackThreshold)
            {
                Utilities::matSet2D(shadowMask,i,j,1);
            }
            else
            {
                Utilities::matSet2D(shadowMask,i,j,0);
            }
        }
    }

    std::cout<<"computeShadows done!\n";
}

void Reconstructor::saveShadowImg(const char path[])
{

    //shadow的计算有两个地方，一个是computeShadows()，一个是decodePatten(),两部分完成保存shadow才有意义。
    if(!shadowMask.empty())
    {
        cv::threshold( shadowMask, shadowMask, 0, 255, cv::THRESH_BINARY);
        cv::imwrite(path,shadowMask);
    }
}


//for a (x,y) pixel of the camera returns the corresponding projector pixel
bool Reconstructor::getProjPixelPattrnIndex(int x, int y, int &grayPattenIndex)
{
    std::vector<bool> grayCol;

    bool result = true;
    //int error_code = 0;
    //int xDec;

    //prosses column images
    for(int count=0; count < numOfColBits; count++)
    {
        //get pixel intensity for regular pattern projection and it's inverse
        double val1, val2;
        val1 = Utilities::matGet2D(camImgs[count * 2 + 2   ],x,y);
        val2 = Utilities::matGet2D(camImgs[count * 2 + 2 + 1],x,y);

        //check if intensity deference is in a valid rage
        if(abs(val1 - val2) > whiteThreshold)
        {
            result = true;
        }
        else
        {
            result = false;
            break;
        }

        //determine if projection pixel is on or off
        if(val1 > val2)
            grayCol.push_back(1);
        else
            grayCol.push_back(0);

    }

    if(result == true)
    {
        grayPattenIndex  = GrayCodes::grayToDec(grayCol);
    }

    return result;
}


void Reconstructor::decodePaterns(VirtualCamera &camera,std::vector<std::vector<cv::Point>> &Pixels)
{

    std::cout<<"Decoding paterns...\n";
    Pixels.clear();
    Pixels.resize(912);
    int w=camera.width;
    int h=camera.height;


    int  pattenIndex;

    for(int i=0; i<w; i++)
    {
        for(int j=0; j<h; j++)
        {

            //if the pixel is not shadow reconstruct
            if(shadowMask.at<uchar>(j,i))
            {

                //get the projector pixel for camera (i,j) pixel
                bool result = getProjPixelPattrnIndex(i,j,pattenIndex);

                if(result == false)
                {
                    shadowMask.at<uchar>(j,i)=0;
                    continue;
                }

               Pixels[pattenIndex].push_back(cv::Point(i,j));

            }
        }
    }
    std::cout<<"decodePaterns done!\n";
}



void Reconstructor::runReconstruction()
{

    this->loadCameras(&(this->cam),"../calibrationData/camera1/");
    this->loadCameras(&(this->projector),"../calibrationData/projector/");


    this->loadCamImgs(&(this->projector), "../Pictures/gray_22/","",".bmp");
    this->computeShadows(this->projector);
    this->saveShadowImg("../Pictures/shadow/shadow0.bmp");
    this->decodePaterns(this->projector,this->projectorPixels);
    std::cout<<"generate3DCloud_1_Thread()"<<std::endl;
    this->saveShadowImg("../Pictures/shadow/shadow1.bmp");


//    this->autoContrast=true;
//    this->saveAutoContrast=true;
    this->loadCamImgs(&(this->cam), "../Pictures/Hardware_trigger_frame/","",".bmp");
    this->computeShadows(this->cam);
    this->saveShadowImg("../Pictures/shadow/shadow2.bmp");
    this->decodePaterns(this->cam,this->camPixels);
    std::cout<<"generate3DCloud_1_Thread()"<<std::endl;
    this->saveShadowImg("../Pictures/shadow/shadow3.bmp");

//    std::cout<<this->camPixels.size()<<"\n"<<this->projectorPixels.size()<<"\n";
//    std::cout<<this->camPixels[0].size()<<"\n"<<this->projectorPixels[0].size()<<"\n";
//    std::cout<<this->camPixels[100].size()<<"\n"<<this->projectorPixels[100].size()<<"\n";
//    std::cout<<this->camPixels[400].size()<<"\n"<<this->projectorPixels[400].size()<<"\n";
//    std::cout<<this->camPixels[600].size()<<"\n"<<this->projectorPixels[600].size()<<"\n";



    //reconstruct
    points3DProjView = new PointCloudImage(this->cam.width, this->cam.height , true );


    triangulation(this->camPixels,this->cam,this->projectorPixels,this->projector);

    std::string temp3DPath="../Pictures/test_3D_2.xyz";


    points3DProjView->exportXYZ(temp3DPath.c_str(),1,1);

    delete points3DProjView;
    points3DProjView=nullptr;



}


//convert a point from camera to world space
void Reconstructor::cam2WorldSpace(VirtualCamera cam, cv::Point3f &p)
{

    cv::Mat tmp(3,1,CV_32F);
    cv::Mat tmpPoint(3,1,CV_32F);

    tmpPoint.at<float>(0) = p.x;
    tmpPoint.at<float>(1) = p.y;
    tmpPoint.at<float>(2) = p.z;

    tmp = -cam.rotationMatrix.t() * cam.translationVector ;
    tmpPoint = cam.rotationMatrix.t() * tmpPoint;

    p.x = tmp.at<float>(0) + tmpPoint.at<float>(0);
    p.y = tmp.at<float>(1) + tmpPoint.at<float>(1);
    p.z = tmp.at<float>(2) + tmpPoint.at<float>(2);

}

void Reconstructor::triangulation(std::vector<std::vector<cv::Point>> &cam1Pixels, VirtualCamera camera1, std::vector<std::vector<cv::Point>> &cam2Pixels, VirtualCamera camera2)
{
    //camera1为相机，camera2为投影仪
    int w = 912;//projector width

    //start reconstraction
    int load=0;

    //find camera Center();
    camera1.position = cv::Point3f(0,0,0);
    cam2WorldSpace(camera1,camera1.position);

    camera2.position = cv::Point3f(0,0,0);
    cam2WorldSpace(camera2,camera2.position);

    //reconstraction for every projector pixel
    for(int i=0; i<w; i++)
    {

              int j=0;
//            if(load != (int)((float)i*100/w) )
//            {
//                load =  (int)((float)i*100/w);
                std::cout<<"Computing 3D Cloud "<<i<< "col\n\n\n\n\n";
//            }

            std::vector<cv::Point> cam1Pixs,cam2Pixs;

            cam1Pixs = cam1Pixels[i];
            cam2Pixs = cam2Pixels[i];

            //求对应的col投影平面的平面表达式（法向量和一个点，点即为投影仪中心点，法向量为平面的法向量）
            cv::Point2f projectorPixel = Utilities::undistortPoints(cv::Point2f(cam2Pixs[0].x,cam2Pixs[0].y),camera2);
            cv::Point3f projectorPoint_1 = Utilities::pixelToImageSpace(projectorPixel,camera2); //convert camera pixel to image space
            cam2WorldSpace(camera2, projectorPoint_1);

            projectorPixel = Utilities::undistortPoints(cv::Point2f(cam2Pixs[cam2Pixs.size()-1].x,cam2Pixs[cam2Pixs.size()-1].y),camera2);
            cv::Point3f projectorPoint_2 = Utilities::pixelToImageSpace(projectorPixel,camera2); //convert camera pixel to image space
            cam2WorldSpace(camera2, projectorPoint_2);


            cv::Vec3f line_cross_c1(projectorPoint_1.x-camera2.position.x,projectorPoint_1.y-camera2.position.y,projectorPoint_1.z-camera2.position.z);
            cv::Vec3f line_cross_c2(projectorPoint_2.x-camera2.position.x,projectorPoint_2.y-camera2.position.y,projectorPoint_2.z-camera2.position.z);
            cv::Vec3f  normal_vector= line_cross_c1.cross(line_cross_c2);
            Utilities::normalize(normal_vector);


            if( cam1Pixs.size() == 0 || cam2Pixs.size() == 0)
            {
                std::cout<<" continue\n";
                continue;
            }

            cv::Vec3f color;


            for(int c1=0; c1 < cam1Pixs.size(); c1++)
            {

                cv::Point2f camPixelUD = Utilities::undistortPoints(cv::Point2f(cam1Pixs[c1].x,cam1Pixs[c1].y),camera1);//camera 3d point p for (i,j) pixel
                cv::Point3f cam1Point = Utilities::pixelToImageSpace(camPixelUD,camera1); //convert camera pixel to image space
                cam2WorldSpace(camera1, cam1Point);

                cv::Vec3f ray1Vector = (cv::Vec3f)(camera1.position - cam1Point); //compute ray vector
                Utilities::normalize(ray1Vector);

                //get pixel color for the first camera view
                color = Utilities::matGet3D( colorImg, cam1Pixs[c1].x, cam1Pixs[c1].y);


               cv::Point3f interPoint = Utilities::planeRayInter(normal_vector,camera2.position, ray1Vector,camera1.position);

                if(interPoint != cv::Point3f(0,0,0))
                {
                    std::cout<<interPoint<<"\t"<<i<<"\t"<<c1<<std::endl;
                    points3DProjView->addPoint(cam1Pixs[c1].x,cam1Pixs[c1].y,interPoint, color);
                }
            }



     }

    system("cls");
    std::cout<<"Computing 3D Cloud 100%\n";


}


