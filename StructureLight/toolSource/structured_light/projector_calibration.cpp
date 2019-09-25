#include "projector_calibration.h"

ProjectorCalibration::ProjectorCalibration()
{

}

void ProjectorCalibration::setProjectorCornersSize(cv::Size size)
{

    this->numOfCornersX_projector=size.width;
    this->numOfCornersY_projector=size.height;
}

cv::Size ProjectorCalibration::getProjectorCornersSize()
{
    return cv::Size(this->numOfCornersX_projector,this->numOfCornersY_projector);
}



void ProjectorCalibration::loadProjectorImgs(const QString ImgPath)
{
    std::cout<<"**************load ProjectorImgs***************"<<std::endl;
    std::cout<<ImgPath.toStdString()<<std::endl;
    projectorImgPath=ImgPath.toStdString();
    projectorImg = cv::imread(ImgPath.toStdString().c_str());

    if( projectorImg.empty())
    {
       std::cout<<"Error loading  projectorImg calibration image!";
       exit(-1);
    }
    if(!projectorImg.empty())
        this->camImageSize = projectorImg.size();

    std::cout<<"loadProjectorImg done!\n";
}



cv::Mat ProjectorCalibration::findCameraHomography_imgTo3D(std::vector<cv::Point3f> &objPoints3D, std::vector<cv::Point2f> &imgPoints)
{
    std::vector<cv::Point2f> objPoints2d(objPoints3D.size());

    for(int i=0; i < objPoints3D.size(); i++)
    {
         objPoints2d[i].x=objPoints3D[i].x;
         objPoints2d[i].y=objPoints3D[i].y;
    }
    cv::Mat homo=cv::findHomography(imgPoints,objPoints2d);

    return homo;
}


void ProjectorCalibration::findCameraAllHomography_imgTo3D()
{
    this->homography.clear();
//    this->extractImageCorners();//����ִ�У���ʱcaliImgs�������Ӧ��ϵ��ͼƬ
    this->homography=std::vector <cv::Mat>(objBoardCornersCam.size());
    for (int i = 0; i < this->objBoardCornersCam.size(); i++)
    {
        homography[i]=this->findCameraHomography_imgTo3D(this->objBoardCornersCam[i], this->imgBoardCornersCam[i]);
    }
    this->unloadCameraImgs();
}


void ProjectorCalibration::reSetProjectParm()
{
    this->numOfCornersX=this->numOfCornersX_projector;
    this->numOfCornersY=this->numOfCornersY_projector;
}

bool ProjectorCalibration::findCornersInCamImg_projector(cv::Mat img,std::vector<cv::Point2f> *camCorners,int threshold_value)
{

    //copy camera img
    cv::Mat img_grey;
    cv::Mat img_copy;
    img.copyTo(img_copy);//img_copy=img.clone();

//    Utilities::autoContrast(img_copy, img_copy);

    //show img to user
    static int img_index=-1;
    img_index++;
    cv::namedWindow("origin_projector_img"+std::to_string(img_index), cv::WINDOW_AUTOSIZE );
    cv::imshow("origin_projector_img"+std::to_string(img_index),img_copy);


    //convert the copy to gray
    cv::cvtColor(img_copy, img_grey, cv::COLOR_BGR2GRAY );




    //������ֵ��ȥ��ԭʼƽ���㣬����ͶӰ���
    int threshold_type = 0;
    int const max_binary_value = 255;
    cv::threshold( img_grey, img_grey,threshold_value, max_binary_value, threshold_type );
    cv::namedWindow("threshold_img"+std::to_string(img_index), cv::WINDOW_AUTOSIZE );
    cv::imshow("threshold_img"+std::to_string(img_index),img_grey);

    cv::waitKey(20);

    camCorners->clear();
    if (cv::findChessboardCorners(img_grey, cv::Size(this->numOfCornersX,this->numOfCornersY), *camCorners, cv::CALIB_CB_ADAPTIVE_THRESH) == false)
    {
         std::cout << "can not find chessboard corners!\n";   //�Ҳ����ǵ�
         return false;
    }
    else
    {
//    cv::find4QuadCornerSubpix(img_grey, *camCorners, cv::Size(5, 5));

        //find sub pix of the corners
        cv::cornerSubPix(img_grey, *camCorners, cv::Size(20,20), cv::Size(-1,-1), cv::TermCriteria((cv::TermCriteria::COUNT)+(cv::TermCriteria::EPS), 30, 0.001));
        cv::drawChessboardCorners(img_copy, cv::Size(this->numOfCornersX,this->numOfCornersY), *camCorners, true);
        cv::imshow("drawChessboardCorners_projector_img"+std::to_string(img_index),img_copy);
        cv::waitKey(20);
    }

//     std::cout<<"**************��������˳�******************"<<std::endl;
//     cv::waitKey(1000);
//     cv::destroyAllWindows();
     return true;
}


bool ProjectorCalibration::extractImageCorners_projector()
{

    //�ͷ�caliImgs�����¼���caliImgs

    reSetProjectParm();

    if(calibImgs.size()==0)
    {
        std::cout<<"Calibration Images are not loaded.\n";
        return 0;
    }

    imgBoardCornersCam.clear();
    objBoardCornersCam.clear();
    std::vector<cv::Point2f> cProjector;
    std::vector<cv::Point2f> cCam;
    std::vector<cv::Point3f> cObj;
    this->findCornersInCamImg( projectorImg,&cProjector);
    for(size_t i=0; i<this->calibImgs.size(); i++)
    {
        //74Ϊ��ֵ
        this->findCornersInCamImg_projector(calibImgs[i], &cCam,74);
        this->perspectiveTransformation(cCam,this->homography[i],cObj);

        if(cObj.size())
        {
//            std::cout<<cCam.size()<<std::endl;
//            std::cout<<cObj.size()<<std::endl;
//            std::cout<<cProjector.size()<<std::endl;
            objBoardCornersCam.push_back(cObj);
            imgBoardCornersCam.push_back(cProjector);
        }

    }
    return true;
}

bool ProjectorCalibration::findProjectorExtrisics(const QString  projectorFilesDir,const QString  cameraFilesDir,std::string  exPath)
{

    std::vector<cv::Point2f> imgPoints;
    std::vector<cv::Point3f> objPoints3D;


    //��ͶӰ��ͶӰ�Ŀռ�3D������
    size_t index = 0;
    std::vector<std::string> projectorFiles;
    std::vector<std::string> cameraFiles;
    Utilities::folderScan(projectorFilesDir,&projectorFiles);
    Utilities::folderScan(cameraFilesDir,&cameraFiles);

    int i=0;
    for(i; i < cameraFiles.size(); i++)
    {
        std::cout<<cameraFiles[i]<<std::endl;
        std::cout<<exPath<<std::endl;

        if(cameraFiles[i] == exPath)
        {
            index = i;
            break;
        }

    }

    if(i == cameraFiles.size())
    {
        std::cout<<"����������ļ�"<<std::endl;
        exit(-1);
    }

    cv::Mat cameraFile=cv::imread(cameraFiles[index].c_str());

    std::vector<cv::Point2f> tempimgPoints;
    std::vector<cv::Point3f> tempobjPoints3D;

    findCornersInCamImg(cameraFile, &tempimgPoints);
    findCornersInObjetBoard(&tempobjPoints3D);

    cv::Mat exHomo=findCameraHomography_imgTo3D(tempobjPoints3D,tempimgPoints);
    //����homo���������ø����
    this->reSetProjectParm();

    //���ͼƬ
    loadCameraExtriImgs(QString::fromStdString(projectorFiles[index]));

    //74Ϊ��ֵ
    this->findCornersInCamImg_projector(extrImg, &imgPoints,74);
    this->perspectiveTransformation(imgPoints,exHomo,objPoints3D);

    //��ͼ��ĵ�
    findCornersInCamImg(projectorImg,&imgPoints);

    //find extrinsics rotation & translation
    bool reslut = cv::solvePnP(objPoints3D,imgPoints,camMatrix,distortion,rotationVector,translationVector);
    cv::Rodrigues(rotationVector,rotationMatrix);

    std::cout<<rotationMatrix<<"\n\n\n"<<translationVector<<"\n\n\n";

    //**********����α궨�����������****************
    std::vector<cv::Point2f> image_points_calculated;//����¼������ͶӰ�������
    std::vector<cv::Point3f> tempPointSet = objPoints3D;
    cv::projectPoints(tempPointSet, rotationVector, translationVector, camMatrix,  distortion, image_points_calculated);

    //�����µ�ͶӰ����ɵ�ͶӰ��֮������
    std::vector<cv::Point2f> image_points_old = imgPoints;

    //���������ݻ���Mat��ʽ
    cv::Mat image_points_calculated_mat = cv::Mat(1, image_points_calculated.size(), CV_32FC2);
    cv::Mat image_points_old_mat = cv::Mat(1,image_points_old.size(), CV_32FC2);
    for (size_t j = 0; j < (tempPointSet.size()); j++)
    {
        image_points_calculated_mat.at<cv::Vec2f>(0, j) = cv::Vec2f(image_points_calculated[j].x, image_points_calculated[j].y);
        image_points_old_mat.at<cv::Vec2f>(0, j) = cv::Vec2f(image_points_old[j].x, image_points_old[j].y);
    }
    err_of_extrImg = cv::norm(image_points_calculated_mat, image_points_old_mat, cv::NORM_L2);//ƽ������û�п�����,������Ҳ���Խ�����ͶӰ���������������궨�����һ��ָ�ꡣ

    err_of_extrImg /= tempPointSet.size();

    std::cout<<total_err<<"\n\n\n"<<err_of_extrImg<<"\n\n\n";
    return reslut;
}





