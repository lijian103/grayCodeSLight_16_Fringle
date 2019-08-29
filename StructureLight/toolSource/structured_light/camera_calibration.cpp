/**@file time_measurement.h
 * @brief contains definitions for the TimeMeasurement Class
 * @copyright lijian
 * data 2019/8/7
 */

#include "camera_calibration.h"
#include "utilities.h"


CameraCalibration::CameraCalibration()
{

}

CameraCalibration::~CameraCalibration()
{

}

void CameraCalibration::setSquareSize(cv::Size2d size)
{
    this->squareSize.width=size.width;
    this->squareSize.height=size.height;
}

cv::Size CameraCalibration::getSquareSize()
{
    return this->squareSize;
}

void CameraCalibration::setCornersSize(cv::Size size)
{

    this->numOfCornersX=size.width;
    this->numOfCornersY=size.height;
}

cv::Size CameraCalibration::getCornersSize()
{
    return cv::Size(this->numOfCornersX,this->numOfCornersY);
}


void CameraCalibration::getImgFilesPath(const QString dirPath)
{
    this->imgFilesPath.clear();
    QFileInfo fileinfo(dirPath);
    if(!fileinfo.isDir())
    {
        std::cout << "没有找到图片文件夹路径："<<dirPath.toStdString()<< std::endl;
//        exit(1);
        return ;
    }
    QDir dir(dirPath);
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png"<< "*.bmp";
    QStringList files= dir.entryList(nameFilters, QDir::NoSymLinks|QDir::Files|QDir::Readable|QDir::NoDotAndDotDot , QDir::Name);
    for (int i=0; i < files.length();i++)
    {
        std::string temp = dirPath.toStdString()+"/"+files[i].toStdString();
        std::cout<<temp<<std::endl;
        this->imgFilesPath.push_back(temp);
    }
}

void CameraCalibration::loadCameraImgs(const QString dirPath)
{
    this->getImgFilesPath(dirPath);
    calibImgs.clear();
    if(this->imgFilesPath.size()==0)
    {

        std::cout<<"没有图片路径：imgFilesPath.size()==0"<<std::endl;
        exit(-1);
    }
    for(size_t i=0; i<imgFilesPath.size();i++)
    {
        std::cout<<"**************load intrinsictsImgs***************"<<std::endl;
        std::cout<<imgFilesPath[i]<<std::endl;
        cv::Mat img = cv::imread(imgFilesPath[i].c_str() );

        if(img.empty())
        {
            std::cout<<"Error loading calibration image! "<<std::endl;
            exit(-1);
        }
        calibImgs.push_back(img);
    }

    if(!calibImgs[0].empty())
        this->camImageSize = calibImgs[0].size();

    std::cout<<"loadCameraImgs done!\n";

}

void CameraCalibration::loadCameraExtriImgs(const QString extrinImgPath)
{
    std::cout<<"**************load extrinsictsImg***************"<<std::endl;
    std::cout<<extrinImgPath.toStdString()<<std::endl;
    extrImg = cv::imread(extrinImgPath.toStdString().c_str());

    if(extrImg.empty())
    {
       std::cout<<"Error loading extrinsicts calibration image!";
       exit(-1);
    }
}



void CameraCalibration::unloadCameraImgs()
{
    for(size_t i=0; i<calibImgs.size();i++)
            calibImgs[i].release();

}

void CameraCalibration::unloadCameraExtriImg()
{
    extrImg.release();
}



 bool CameraCalibration::findCornersInCamImg(cv::Mat img,std::vector<cv::Point2f> *camCorners)
{
    //copy camera img
    cv::Mat img_grey;
    cv::Mat img_copy;
    //convert the copy to gray
    cv::cvtColor( img, img_grey, cv::COLOR_BGR2GRAY );
    img.copyTo(img_copy);//img_copy=img.clone();


    //show img to user
    static int img_index=-1;
    img_index++;
    cv::namedWindow("origin_img"+std::to_string(img_index), cv::WINDOW_AUTOSIZE );
    cv::imshow("origin_img"+std::to_string(img_index),img);

    cv::waitKey(20);


    if (cv::findChessboardCorners(img_grey, cv::Size(this->numOfCornersX,this->numOfCornersY), *camCorners, cv::CALIB_CB_ADAPTIVE_THRESH) == false)
    {
         std::cout << "can not find chessboard corners!\n";   //找不到角点
         return false;
    }
    else
    {
//    cv::find4QuadCornerSubpix(img_grey, *camCorners, cv::Size(5, 5));

        //find sub pix of the corners
        cv::cornerSubPix(img_grey, *camCorners, cv::Size(20,20), cv::Size(-1,-1), cv::TermCriteria((cv::TermCriteria::COUNT)+(cv::TermCriteria::EPS), 30, 0.001));
        cv::drawChessboardCorners(img_copy, cv::Size(this->numOfCornersX,this->numOfCornersY), *camCorners, true);
        cv::imshow("drawChessboardCorners"+std::to_string(img_index),img_copy);
        cv::waitKey(20);
    }

//     std::cout<<"**************按任意键退出******************"<<std::endl;
//     cv::waitKey(1000);
//     cv::destroyAllWindows();
     return true;
}


 bool CameraCalibration::findCornersInObjetBoard(std::vector<cv::Point3f> *objCorners)
{
     if(squareSize.height == 0 && squareSize.height==0)
     {
         std::cout<<"Give square height in mm: \n";
         std::cout<<"Give square width in mm: \n";
         return false;
     }

     objCorners->clear();
     for(int i=0; i<numOfCornersY ; i++)
     {
         for(int j=0; j<numOfCornersX; j++)
         {
             cv::Point3f p;
             p.x = j*squareSize.width;
             p.y = i*squareSize.height;
             p.z = 0;
             objCorners->push_back(p);
         }
     }
     return true;
}


 bool CameraCalibration::extractImageCorners()
 {

     if(calibImgs.size()==0)
     {
         std::cout<<"Calibration Images are not loaded.\n";
         return 0;
     }

     imgBoardCornersCam.clear();
     objBoardCornersCam.clear();
     std::vector<cv::Point2f> cCam;
     std::vector<cv::Point3f> cObj;
     for(size_t i=0; i<this->calibImgs.size(); i++)
     {

         findCornersInCamImg(calibImgs[i], &cCam);
         findCornersInObjetBoard(&cObj);

         if(cCam.size())
         {
             imgBoardCornersCam.push_back(cCam);
             objBoardCornersCam.push_back(cObj);
         }

     }

     return true;
 }



 bool CameraCalibration::calibrateCamera()
 {
     //check if corners for camera calib has been extracted
     if(imgBoardCornersCam.size() == 0)
     {
        extractImageCorners();
     }

     std::vector<cv::Mat> camRotationVectors;
     std::vector<cv::Mat> camTranslationVectors;


     cv::calibrateCamera(objBoardCornersCam,imgBoardCornersCam,camImageSize,camMatrix, distortion, camRotationVectors,camTranslationVectors,0,
     cv::TermCriteria( (cv::TermCriteria::COUNT)+(cv::TermCriteria::EPS), 30, DBL_EPSILON) );


     //优化内参
//   cv::Mat optimalNewCameraMatrix=cv::getOptimalNewCameraMatrix(camMatrix, distortion, camImageSize, 0);

     //**********对标定结果进行评价****************
     std::vector <double> temp(calibImgs.size(),0.0);
     err_of_each_calibImgs=temp;
     total_err=0;
     for(size_t i=0; i<this->calibImgs.size(); i++)
     {
         std::vector<cv::Point2f> image_points_calculated;//存放新计算出的投影点的坐标

         std::vector<cv::Point3f> tempPointSet = objBoardCornersCam[i];

         cv::projectPoints(tempPointSet, camRotationVectors[i], camTranslationVectors[i], camMatrix,  distortion, image_points_calculated);

         //计算新的投影点与旧的投影点之间的误差
         std::vector<cv::Point2f> image_points_old = imgBoardCornersCam[i];

         //将两组数据换成Mat格式
         cv::Mat image_points_calculated_mat = cv::Mat(1, image_points_old.size(), CV_32FC2);
         cv::Mat image_points_old_mat = cv::Mat(1,(int)image_points_old.size(), CV_32FC2);
         for (size_t j = 0; j < (tempPointSet.size()); j++)
         {
             image_points_calculated_mat.at<cv::Vec2f>(0, j) = cv::Vec2f(image_points_calculated[j].x, image_points_calculated[j].y);
             image_points_old_mat.at<cv::Vec2f>(0, j) = cv::Vec2f(image_points_old[j].x, image_points_old[j].y);
         }

         err_of_each_calibImgs[i] = cv::norm(image_points_calculated_mat, image_points_old_mat, cv::NORM_L2);//平方和误差（没有开方）,这个误差也可以叫做重投影误差，这个是评价相机标定结果的一个指标。

         err_of_each_calibImgs[i] /= tempPointSet.size();


         total_err +=  err_of_each_calibImgs[i];
     }

     total_err /= this->calibImgs.size();

     camCalibrated = true;

     std::cout<<camMatrix<<"\n\n\n"<<distortion<<"\n\n\n"<<total_err<<"\n\n\n";

     return 1;
 }


 bool CameraCalibration::findCameraExtrisics()
 {

     std::vector<cv::Point2f> imgPoints;
     std::vector<cv::Point3f> objPoints3D;

     findCornersInCamImg(extrImg, &imgPoints);
     findCornersInObjetBoard(&objPoints3D);

     //find extrinsics rotation & translation
     bool reslut = cv::solvePnP(objPoints3D,imgPoints,camMatrix,distortion,rotationVector,translationVector);
     cv::Rodrigues(rotationVector,rotationMatrix);

     std::cout<<rotationMatrix<<"\n\n\n"<<translationVector<<"\n\n\n";

     //**********对外参标定结果进行评价****************
     std::vector<cv::Point2f> image_points_calculated;//存放新计算出的投影点的坐标
     std::vector<cv::Point3f> tempPointSet = objPoints3D;
     cv::projectPoints(tempPointSet, rotationVector, translationVector, camMatrix,  distortion, image_points_calculated);

     //计算新的投影点与旧的投影点之间的误差
     std::vector<cv::Point2f> image_points_old = imgPoints;

     //将两组数据换成Mat格式
     cv::Mat image_points_calculated_mat = cv::Mat(1, image_points_calculated.size(), CV_32FC2);
     cv::Mat image_points_old_mat = cv::Mat(1,image_points_old.size(), CV_32FC2);
     for (size_t j = 0; j < (tempPointSet.size()); j++)
     {
         image_points_calculated_mat.at<cv::Vec2f>(0, j) = cv::Vec2f(image_points_calculated[j].x, image_points_calculated[j].y);
         image_points_old_mat.at<cv::Vec2f>(0, j) = cv::Vec2f(image_points_old[j].x, image_points_old[j].y);
     }
     err_of_extrImg = cv::norm(image_points_calculated_mat, image_points_old_mat, cv::NORM_L2);//平方和误差（没有开方）,这个误差也可以叫做重投影误差，这个是评价相机标定结果的一个指标。

     err_of_extrImg /= tempPointSet.size();

     std::cout<<total_err<<"\n\n\n"<<err_of_extrImg<<"\n\n\n";
     return reslut;
 }


 bool CameraCalibration::undistortionImg(const QString distortionImgPath)
 {
     cv::Mat src_image = cv::imread(distortionImgPath.toStdString().c_str(), 1);
     cv::Mat new_image = src_image.clone();

     QFileInfo fileinfo(distortionImgPath);


     cv::Mat mapx = cv::Mat(camImageSize, CV_32FC1);
     cv::Mat mapy = cv::Mat(camImageSize, CV_32FC1);
     cv::Mat R = cv::Mat::eye(3, 3, CV_32F);

     cv::Mat optimalNewCameraMatrix=cv::getOptimalNewCameraMatrix(camMatrix, distortion, camImageSize, 0);
     cv::initUndistortRectifyMap(camMatrix, distortion, R, optimalNewCameraMatrix, camImageSize, CV_32FC1, mapx, mapy);
     cv::remap(src_image, new_image, mapx, mapy, cv::INTER_LINEAR);
     cv::imwrite((fileinfo.absolutePath()+"/"+fileinfo.fileName()+"distortion"+fileinfo.suffix()).toStdString().c_str(), new_image);
     return true;

 }


 void CameraCalibration::undistortCameraImgPoints(std::vector<cv::Point2f> points_in,std::vector<cv::Point2f> &points_out)
 {
     cv::undistortPoints(points_in,points_out,camMatrix,distortion);

     float fX = camMatrix.at<double>(0,0);
     float fY = camMatrix.at<double>(1,1);
     float cX = camMatrix.at<double>(0,2);
     float cY = camMatrix.at<double>(1,2);

     for(int j=0; j<points_out.size(); j++)
     {

         points_out[j].x = (points_out[j].x*fX)+cX;
         points_out[j].y = (points_out[j].y*fY)+cY;

     }
 }

 void CameraCalibration::perspectiveTransformation(std::vector<cv::Point2f> corners_in,cv::Mat homoMatrix, std::vector<cv::Point3f> &points_out)
 {
    points_out.clear();

    //opencv 求homo是double，VirtualCamera类里面求投影矩阵是float
    int type = homoMatrix.type();
    if(type==CV_32F)
    {

     for(int i=0; i<corners_in.size(); i++)
     {
         cv::Point3f p;

         double x = corners_in[i].x, y = corners_in[i].y;

         double Z = 1./(homoMatrix.at<float>(6) *x + homoMatrix.at<float>(7)*y + homoMatrix.at<float>(8));
         double X =    (homoMatrix.at<float>(0) *x + homoMatrix.at<float>(1)*y + homoMatrix.at<float>(2))*Z;
         double Y =    (homoMatrix.at<float>(3) *x + homoMatrix.at<float>(4)*y + homoMatrix.at<float>(5))*Z;

         p.x = (float) X;
         p.y = (float) Y;
         p.z = 0;

         points_out.push_back(p);

     }
    }else if(type==CV_64F)
    {
        for(int i=0; i<corners_in.size(); i++)
        {
            cv::Point3f p;

            double x = corners_in[i].x, y = corners_in[i].y;

            double Z = 1./(homoMatrix.at<double>(6) *x + homoMatrix.at<double>(7)*y + homoMatrix.at<double>(8));
            double X =    (homoMatrix.at<double>(0) *x + homoMatrix.at<double>(1)*y + homoMatrix.at<double>(2))*Z;
            double Y =    (homoMatrix.at<double>(3) *x + homoMatrix.at<double>(4)*y + homoMatrix.at<double>(5))*Z;

            p.x = (float) X;
            p.y = (float) Y;
            p.z = 0;

            points_out.push_back(p);

        }
    }


 }


 void CameraCalibration::exportTxtFiles(const char *path, int CAMCALIB_OUT_PARAM)
 {
     cv::Mat out;
     switch (CAMCALIB_OUT_PARAM)
     {
         case CAMCALIB_OUT_MATRIX:
             out = camMatrix;
             break;
         case CAMCALIB_OUT_DISTORTION:
             out = distortion;
             break;
         case CAMCALIB_OUT_ROTATION:
             out = rotationMatrix;
             break;
         case CAMCALIB_OUT_TRANSLATION:
             out = translationVector;
             break;
        case CAMCALIB_OUT_ERROR:
             out =(cv::Mat_<double>(2,1) <<this->total_err, this->err_of_extrImg);
             break;
     }

     Utilities::exportMat(path, out);


 }






 //----------------------------------------Load & Export Data--------------------------------------


 void CameraCalibration::loadCalibData(const char *path)
 {
     cv::FileStorage fs(path, cv::FileStorage::READ);

     if(!fs.isOpened())
     {
         std::cout << "Failed to open Calibration Data File. " << std::endl;
         return;
     }

     cv::FileNode node = fs["Camera"];
         node["Calibrated"] >> camCalibrated;
         node["Matrix"] >> camMatrix;
         node["Distortion"]>> distortion;
         node["Rotation"]>> rotationMatrix;
         node["Translation"] >> translationVector;
         node["Height"] >> camImageSize.height;
         node["Width"] >> camImageSize.width;
         node["total_err"] >> total_err;
         node["err_of_extrImg"] >> err_of_extrImg;

     node = fs["BoardSquare"];

         node["Height"] >> squareSize.height;
         node["Width"] >> squareSize.width;

     node = fs["NumOfCorners_for_each_img"];

         node["numOfCornersX"] >> numOfCornersX;
         node["numOfCornersY"] >> numOfCornersY;


     cv::FileNode features = fs["ExtractedFeatures"];

         cv::FileNode images = features["CameraImages"];
             int size = images["NumberOfImgs"];

                 for(int i=0; i<size; i++)
                 {
                     std::stringstream name;
                     name << "Image" << i+1;

                     cv::FileNode image = images[name.str()];

                     std::vector<cv::Point2f> in2;
                     std::vector<cv::Point3f> in3;

                     image["BoardCorners"]>>in2;
                     imgBoardCornersCam.push_back(in2);

                     image["ObjBoardCorners"]>>in3;
                     objBoardCornersCam.push_back(in3);

                 }

     fs.release();
 }


 void CameraCalibration::saveCalibData(const char *path)
 {
     cv::FileStorage fs(path, cv::FileStorage::WRITE);

     fs << "Camera" << "{:";
     fs<< "Calibrated" << camCalibrated << "Matrix" << camMatrix << "Distortion" << distortion<<"Translation"<<translationVector<<"Rotation"<<rotationMatrix;
         fs<<"Height" << camImageSize.height<<"Width" << camImageSize.width<<"total_err" << total_err<<"err_of_extrImg" <<err_of_extrImg;
     fs<<"}";


     fs << "BoardSquare" << "{:";
         fs << "Height" << squareSize.height << "Width" << squareSize.width;
     fs<<"}";

     fs << "NumOfCorners_for_each_img" << "{:";
         fs << "numOfCornersX" << numOfCornersX << "numOfCornersY" << numOfCornersY;
     fs<<"}";

     fs << "ExtractedFeatures" << "{:";

         fs << "CameraImages" << "{:";

             int size = imgBoardCornersCam.size();
             fs << "NumberOfImgs" << size;

                 for(int i=0; i<imgBoardCornersCam.size(); i++)
                 {

                     std::stringstream name;
                     name << "Image" << i+1;
                     fs<<name.str()<< "{:";

                         fs<<"BoardCorners"<<imgBoardCornersCam[i];
                         fs<<"ObjBoardCorners"<<objBoardCornersCam[i];

                     fs<<"}";

                 }
         fs<<"}";
     fs<<"}";

     fs.release();
 }


void CameraCalibration::loadCameraConfig(const char *path,std::vector<std::string> &cameraParam)
{
    cv::FileStorage fs(path, cv::FileStorage::READ);

    if(!fs.isOpened())
    {
        std::cout << "Failed to open Calibration Data File. " << std::endl;
        return;
    }
    cv::FileNode node = fs["NumOfCorners_for_each_img"];
    node["numOfCornersX"] >> cameraParam[0];
    node["numOfCornersY"] >> cameraParam[1];

    node = fs["BoardSquare"];
    node["Height"] >> cameraParam[2];
    node["Width"] >> cameraParam[3];

    node = fs["calibrationPath"];
    node["calibPath"] >> cameraParam[4];
    node["exCalibPath"] >> cameraParam[5];

    if(cameraParam.size() == 10)
    {
        node = fs["ProjectorImgCorners"];
        node["numOfProjectorImgCornersX"] >> cameraParam[6];
        node["numOfProjectorImgCornersY"] >> cameraParam[7];

        node = fs["ProjectImgPath"];
        node["Path"] >> cameraParam[8];

        node = fs["ProjectHomoDir"];
        node["DirPath"] >> cameraParam[9];
    }


    fs.release();
}

void CameraCalibration::saveCameraConfig(const char *path, std::vector<std::string> &cameraParam)
{
    cv::FileStorage fs(path, cv::FileStorage::WRITE);

    fs << "NumOfCorners_for_each_img" << "{:";
        fs << "numOfCornersX" << cameraParam[0] << "numOfCornersY" << cameraParam[1];
    fs<<"}";

    fs << "BoardSquare" << "{:";
        fs << "Height" << cameraParam[2] << "Width" << cameraParam[3];
    fs<<"}";

    fs << "calibrationPath" << "{:";
        fs << "calibPath" << cameraParam[4] << "exCalibPath" << cameraParam[5];
    fs<<"}";

    if(cameraParam.size() == 10)
    {
        fs << "ProjectorImgCorners" << "{:";
            fs << "numOfProjectorImgCornersX" << cameraParam[6] << "numOfProjectorImgCornersY" << cameraParam[7];
        fs<<"}";

        fs << "ProjectImgPath" << "{:";
            fs << "Path" << cameraParam[8];
        fs<<"}";

        fs << "ProjectHomoDir" << "{:";
            fs << "DirPath" << cameraParam[9];
        fs<<"}";

    }

    fs.release();
}










//-----------------------------------------mouse callbacks for calibration--------------------------------------------

//callback to choose 4 corners on calibration board
void calib_board_corners_mouse_callback( int event, int x, int y, int flags, void* param )
{

    std::vector<cv::Point2f> *corners= (std::vector<cv::Point2f>*) param;

    int ev = event;

    switch( event )
    {

        case cv::EVENT_LBUTTONDOWN:
            if(corners->size() ==4)
                break;
            corners->push_back(cv::Point(x,y));
            break;

    }
}

//return image point
void image_point_return( int event, int x, int y, int flags, void* param )
{

    cv::Scalar *point= (cv::Scalar*) param;

    switch( event )
    {
        case cv::EVENT_LBUTTONDOWN:

            point->val[0]=x;
            point->val[1]=y;
            point->val[2]=1L;
            break;
    }

}




//--------------------------------------------------------------------------------------------------------------------



//allow user to select a rectangular area in the image returning the for corners of the area
std::vector<cv::Point2f>  CameraCalibration::manualMarkCheckBoard(cv::Mat img)
{

    std::vector<cv::Point2f> corners;

    cv::namedWindow("Mark Calibration Board",cv::WINDOW_NORMAL);
    cv::resizeWindow("Mark Calibration Board",800,600);

    //Set a mouse callback
    cv::setMouseCallback( "Mark Calibration Board",calib_board_corners_mouse_callback, (void*) &corners);

    bool ok = false;

    while(!ok)
    {
        corners.clear();
        cv::resizeWindow("Mark Calibration Board",800,600);

        int curNumOfCorners=0;

        cv::Mat img_copy ;
        img.copyTo(img_copy);

        system("cls");

        std::cout<<"Please click on the 4 extrime corners of the board, starting from the top left corner\n";

        cv::Point2f rectSize(20,20);

        while(corners.size()<4)
        {
            //draw selected corners and conection lines
            if(curNumOfCorners < corners.size())
            {
                size_t s = corners.size();

                cv::rectangle(img_copy,	corners[s-1] - rectSize,corners[s-1] + rectSize,cv::Scalar(0,0,255),3);

                if(!(corners.size()==1))
                {
                    cv::line(img_copy, corners[s-1],corners[s-2],cv::Scalar(0,0,255),3);
                }

                curNumOfCorners++;

            }

            cv::imshow("Mark Calibration Board", img_copy);
            cv::waitKey(2);
        }

        //Draw corners and lines
        cv::rectangle( img_copy,	corners[3] - rectSize, corners[3] + rectSize, cv::Scalar(0,0,255), 3);
        cv::line(img_copy, corners[3],corners[2],cv::Scalar(0,0,255),10);
        cv::line(img_copy, corners[3],corners[0],cv::Scalar(0,0,255),10);

        system("cls");
        std::cout<<"Press 'Enter' to continue or 'ESC' to select a new area!\n";

        char key = 0;

        //wait for enter(13) or esc(27) key press
        while( key!=27 && key!=13 )
        {
            cv::imshow("Mark Calibration Board", img_copy );
            key = (char)cv::waitKey();
        }

        //if enter(13) set ok as true to stop the loop or repeat the selection process
        if(key == 13)
            ok = true;
        else
            ok = false;

        img_copy.release();

    }

    cv::destroyWindow("Mark Calibration Board");

    return corners;
}





float CameraCalibration::markWhite(cv::Mat img)
{

        float white;
        cv::namedWindow("Mark White",cv::WINDOW_NORMAL);
        cv::resizeWindow("Mark White",800,600);

        cv::Scalar point;

        // Set a mouse callback
        cv::setMouseCallback( "Mark White",image_point_return, (void*) &point);

        bool ok = false;

        while(!ok)
        {
            cv::Mat img_copy;
            img.copyTo(img_copy);

            cv::resizeWindow("Mark White",800,600);

            int pointsCount=0;
            point.val[2]=0;

            while(pointsCount==0)
            {
                if(point.val[2] == 1)
                {
                    cv::rectangle(img_copy, cv::Point(point.val[0]-10,point.val[1]-10),cv::Point(point.val[0]+10,point.val[1]+10),cv::Scalar(0,0,255),3);

                    white = img.at<uchar>(point.val[1],point.val[0]);

                    pointsCount++;
                    point.val[2]=0;
                }

                cv::imshow("Mark White", img_copy );
                cv::waitKey(2);
            }


            int key = 0;

            while(key != 27 && key != 13)
            {
                cv::imshow("Mark White", img_copy );
                key=cv::waitKey();
            }

            if(key==13)
                ok=true;
            else
                ok=false;

            img_copy.release();
        }

        cv::destroyWindow("Mark White");


        return white;
}

