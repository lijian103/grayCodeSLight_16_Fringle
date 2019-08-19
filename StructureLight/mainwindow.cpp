#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtCore>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <string>
#include <Windows.h>
#include <iostream>
#include "Inc/Camera.h"

//camraCalibration
#include "toolSource/structured_light/virtualCamera.h"
#include "toolSource/structured_light/camera_calibration.h"


using namespace cv;
std::mutex myMutex;//线程锁
int grapFlag = 1;
QImage disImage = QImage(10,10,QImage::Format_Grayscale8);
//初始化静态成员变量
int MainWindow::cameraMode = 0;
int MainWindow::cameraModeOlder = 0;
int MainWindow::thread_mode = 1;
cv::Mat MainWindow::openCvImage = cv::Mat(10,10,CV_8UC1);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //ui->tabWidget->setStyleSheet("QTabBar::tab:selected {color: blue;}");

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpDate()));
    timer->start(1000);

    QTimer *timerShowGrapPictures = new QTimer(this);
    connect(timerShowGrapPictures,SIGNAL(timeout()),this,SLOT(showGrapPictures()));
    timerShowGrapPictures->start(30);

    QTimer *timerShowHardwarePictures = new QTimer(this);
    connect(timerShowHardwarePictures,SIGNAL(timeout()),this,SLOT(showHardwarePictures()));
    timerShowHardwarePictures->start(3000);

    QTimer *timerCheckBuffersInQueue_total = new QTimer(this);
    connect(timerCheckBuffersInQueue_total,SIGNAL(timeout()),this,SLOT(checkBuffersInQueue_total()));
    timerCheckBuffersInQueue_total->start(2000);


   ui->tableWidget_camDistortion->setColumnWidth(0,60);
   ui->tableWidget_camDistortion->setColumnWidth(1,60);
   ui->tableWidget_camDistortion->setColumnWidth(2,60);
   ui->tableWidget_camDistortion->setColumnWidth(3,60);
   ui->tableWidget_camDistortion->setColumnWidth(4,60);
   ui->tableWidget_camSetting->setColumnWidth(0,80);
   ui->tableWidget_camSetting->setColumnWidth(1,80);
   ui->tableWidget_camSetting->setColumnWidth(2,80);
   ui->tableWidget_camSetting->setColumnWidth(3,80);




   ui->tableWidget_camDistortion_2->setColumnWidth(0,60);
   ui->tableWidget_camDistortion_2->setColumnWidth(1,60);
   ui->tableWidget_camDistortion_2->setColumnWidth(2,60);
   ui->tableWidget_camDistortion_2->setColumnWidth(3,60);
   ui->tableWidget_camDistortion_2->setColumnWidth(4,60);
   ui->tableWidget_camSetting_2->setColumnWidth(0,80);
   ui->tableWidget_camSetting_2->setColumnWidth(1,80);
   ui->tableWidget_camSetting_2->setColumnWidth(2,80);
   ui->tableWidget_camSetting_2->setColumnWidth(3,80);



   ui->tableWidget_camDistortion_3->setColumnWidth(0,60);
   ui->tableWidget_camDistortion_3->setColumnWidth(1,60);
   ui->tableWidget_camDistortion_3->setColumnWidth(2,60);
   ui->tableWidget_camDistortion_3->setColumnWidth(3,60);
   ui->tableWidget_camDistortion_3->setColumnWidth(4,60);


//this->showMinimized();
    //开启一个线程

//    this->updateCameraParm=

    this->new_thread=new std::thread(&MainWindow::TestThread_1,this);
    this->new_thread->detach();

    //**************Button
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),
                this,SLOT(generate3DCloud()));

    //使用工具生成的槽，已经连接，再次连接会导致槽函数执行两次。
//    connect(ui->pushButton_cam_intri,SIGNAL(clicked(bool)),
//               this,SLOT(on_pushButton_cam_intri_clicked()));
//    connect(ui->pushButton_camWriteSetting,SIGNAL(clicked(bool)),
//                this,SLOT(on_pushButton_camWriteSetting_clicked()));
//    connect(ui->pushButton_camReadSetting,SIGNAL(clicked(bool)),
//                this,SLOT(on_pushButton_camReadSetting_clicked()));


    //**************Radion Button
    groupButton1=new QButtonGroup(this);
    groupButton1->addButton(ui->radioButton_1,0);
    groupButton1->addButton(ui->radioButton_2,1);
    groupButton1->addButton(ui->radioButton_3,2);
    groupButton1->addButton(ui->radioButton_4,3);
    connect(ui->radioButton_1,SIGNAL(clicked(bool)),
                this,SLOT(cameraModeChoose()));
    connect(ui->radioButton_2,SIGNAL(clicked(bool)),
            this,SLOT(cameraModeChoose()));
    connect(ui->radioButton_3,SIGNAL(clicked(bool)),
            this,SLOT(cameraModeChoose()));
    connect(ui->radioButton_4,SIGNAL(clicked(bool)),
            this,SLOT(cameraModeChoose()));

    //**************check BOX
    connect(ui->checkBox,SIGNAL( stateChanged(int)),
            this,SLOT(getStartOrOff_UsbCamera(int)));

    //**************grap pictures Button***************
    connect(ui->pushButton,SIGNAL(clicked(bool)),
                this,SLOT(grapPictures()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),
            this,SLOT(saveFourPictures()));
    connect(ui->toolButton,SIGNAL(clicked(bool)),
            this,SLOT(selectDirPathDialg()));
    connect(ui->toolButton_2,SIGNAL(clicked(bool)),
            this,SLOT(selectDirPathDialg_grapMode()));





    std::vector<std::string> cameraParam(6,"#####");
    CameraCalibration::loadCameraConfig("../calibrationData/camera1/input/camConfig.xml",cameraParam);

    for(size_t i = 0; i < cameraParam.size()-2; i++)
    {
         ui->tableWidget_camSetting->setItem(1,i,new QTableWidgetItem(QString::fromStdString(cameraParam[i])));
    }
    ui->lineEdit_cam_intri->setText(QString::fromStdString(cameraParam[4]));
    ui->lineEdit_cam_extri->setText(QString::fromStdString(cameraParam[5]));
}

MainWindow::~MainWindow()
{
    delete this->new_thread;
    delete this->ui;
}

void MainWindow::timerUpDate()
{
    QDateTime time = QDateTime::currentDateTime();
    //获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    //设置系统时间显示格式
    ui->label->setText(str);

}

void MainWindow::checkBuffersInQueue_total()
{
    if(MainWindow::cameraMode == 2)
    {
        static int  old_nBuffersInQueue_total_1= 0;
        static int  old_nBuffersInQueue_total_2= 0;
        static int  old_nBuffersInQueue_total_3= 0;
        if(nBuffersInQueue_total != 0)
        {
            if((nBuffersInQueue_total==old_nBuffersInQueue_total_1)&&(nBuffersInQueue_total==old_nBuffersInQueue_total_2)&&(nBuffersInQueue_total==old_nBuffersInQueue_total_3))
            {
                nBuffersInQueue_total=0;
                image_id = 0;
            }
        }
        old_nBuffersInQueue_total_3 = old_nBuffersInQueue_total_2;
        old_nBuffersInQueue_total_2 =old_nBuffersInQueue_total_1;
        old_nBuffersInQueue_total_1 = nBuffersInQueue_total;

    }
}

void MainWindow::getStartOrOff_UsbCamera(int state)
{
    if (state == Qt::Checked) // "选中"
    {
        //从摄像头捕获视频
        if(videoCap.open(0))
        {
            MainWindow::openCvImage = Mat::zeros(videoCap.get(CAP_PROP_FRAME_HEIGHT), videoCap.get(CAP_PROP_FRAME_WIDTH), CV_8UC3);
            qDebug() <<"********************USB camera open********************"<<endl;
        }

    }
    else // 未选中 - Qt::Unchecked
    {
        videoCap.release();
        qDebug() <<"********************USB camera release********************"<<endl;
    }

}


void MainWindow::showGrapPictures()
{
    if(MainWindow::cameraMode == 0)
    {
        if(ui->checkBox->isChecked()==true)
        {
            videoCap>>MainWindow::openCvImage;
            if(MainWindow::openCvImage.data)
            {
                cvtColor(MainWindow::openCvImage, MainWindow::openCvImage, cv::COLOR_BGR2GRAY);//Qt中支持的是RGB图像, OpenCV中支持的是BGR
                QImage disImage = QImage((uchar*)(MainWindow::openCvImage.data), MainWindow::openCvImage.cols, MainWindow::openCvImage.rows, QImage::Format_Grayscale8);
                ui->label_9->setPixmap(QPixmap::fromImage(disImage));
            }
        }
    }



    myMutex.lock();
    if(MainWindow::cameraMode == 1 && grapFlag == 0 && MainWindow::cameraModeOlder == 1)
    {
           disImage = QImage((const unsigned char*)(MainWindow::openCvImage.data),MainWindow::openCvImage.cols,MainWindow::openCvImage.rows,QImage::Format_Grayscale8);
//         QGraphicsScene *scene = new QGraphicsScene;//图像显示
//         scene->addPixmap(QPixmap::fromImage(disImage));
//         ui->graphicsView->setScene(scene);
//         ui->graphicsView->show();
         //ui->label_9->setPixmap(QPixmap::fromImage(disImage.scaled(ui->label_9->size(), Qt::KeepAspectRatio)));
           ui->label_9->setPixmap(QPixmap::fromImage(disImage));
    }

    myMutex.unlock();
}

void MainWindow::grapPictures()
{
    static int index = 0;
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyyMMddhhmmss_");
//    string filename = "../../../Pictures/Grap_frame/";
    QString path = ui->lineEdit->text();
    QDir temp;
    bool exist = temp.exists(path);
    if(!exist)
    {
       bool ok = temp.mkdir(path);
       index=0;
       if( ok )
           qDebug()<<"create Dir ok!!!";
    }
    string filename = path.toStdString();
    filename=filename+"/"+str.toStdString()+std::to_string(index)+".bmp";
    cv::imwrite(filename,MainWindow::openCvImage);
    index++;
}

void MainWindow::saveFourPictures()
{
    QString path = ui->lineEdit_2->text();
    QDir temp;
    bool exist = temp.exists(path);
    if(!exist)
    {
       bool ok = temp.mkdir(path);
       if( ok )
           qDebug()<<"create Dir ok!!!";
    }

    QDir dir("../Pictures/Hardware_trigger_frame");
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png"<<"*.bmp";
    QStringList files = dir.entryList(nameFilters, QDir::NoSymLinks|QDir::Dirs|QDir::Files|QDir::Readable|QDir::NoDotAndDotDot , QDir::Name);
    foreach (QString var, files)
    {
        qDebug()<<"../Pictures/Hardware_trigger_frame/"+var;
        if(QFile::exists(path+"/"+var))
        {
            QFile::remove(path+"/"+var) ;
        }
        QFile::copy("../Pictures/Hardware_trigger_frame/"+var, path+"/"+var);
    }

}

void MainWindow::selectDirPathDialg()
{
    QString file_path = QFileDialog::getExistingDirectory(this,QString::fromStdWString(L"请选择文件夹路径"), "../../../Pictures/Hardware_trigger_frame/");
    ui->lineEdit_2->setText(file_path);
}

void MainWindow::selectDirPathDialg_grapMode()
{
    QString file_path = QFileDialog::getExistingDirectory(this,QString::fromStdWString(L"请选择文件夹路径"), "../../../Pictures/");
    ui->lineEdit->setText(file_path);
}

void MainWindow::cameraModeChoose()
{
      switch(this->groupButton1->checkedId())
      {
          case 0:
              MainWindow::cameraMode = 0;
              ui->tabWidget->setCurrentIndex(0);
              break;
          case 1:
              MainWindow::cameraMode = 1;
              ui->tabWidget->setCurrentIndex(0);
              break;
          case 2:
              MainWindow::cameraMode = 2;
              ui->tabWidget->setCurrentIndex(3);
              break;
          case 3:
              ui->tabWidget->setCurrentIndex(0);
              MainWindow::cameraMode = 3;
              break;
          default:
               qDebug() <<"default"<<endl;
               break;
      }


}

void MainWindow::showHardwarePictures()
{    
    //读取文件，在文件存储的时候加锁，避免存储的同时，进行文件的读取
    if((ui->tabWidget->currentIndex()==1)&&(this->cameraParamReadble ==true))
    {
        //updata cam/projector param
        VirtualCamera cam_1;
        cam_1.loadCameraMatrix("../calibrationData/camera1/output/cam_matrix.txt");
        cam_1.loadDistortion("../calibrationData/camera1/output/cam_distortion.txt");
        cam_1.loadRotationMatrix("../calibrationData/camera1/output/cam_rotation_matrix.txt");
        cam_1.loadTranslationVector("../calibrationData/camera1/output/cam_trans_vectror.txt");
        cam_1.loadCalibrationError("../calibrationData/camera1/output/cam_error.txt");

        for(int i=0;i<cam_1.cameraMatrix.rows;i++)
        {
            for(int j=0;j<cam_1.cameraMatrix.cols;j++)
            {             
                ui->tableWidget_camMatrix->setItem(i,j,new QTableWidgetItem(QString("%1").arg(cam_1.cameraMatrix.at<float>(i,j))));
            }
        }

        for(int i=0;i<cam_1.rotationMatrix.rows;i++)
        {
            for(int j=0;j<cam_1.rotationMatrix.cols;j++)
            {
                ui->tableWidget_camRotationMatrix->setItem(i,j,new QTableWidgetItem(QString("%1").arg(cam_1.rotationMatrix.at<float>(i,j))));

            }
        }

        for(int i=0;i<cam_1.translationVector.rows;i++)
        {
            for(int j=0;j<cam_1.translationVector.cols;j++)
            {
                ui->tableWidget_camTranslationVector->setItem(i,j,new QTableWidgetItem(QString("%1").arg(cam_1.translationVector.at<float>(i,j))));

            }
        }

        for(int i=0;i<cam_1.distortion.rows;i++)
        {
            for(int j=0;j<cam_1.distortion.cols;j++)
            {
                ui->tableWidget_camDistortion->setItem(j+1,i,new QTableWidgetItem(QString("%1").arg(cam_1.distortion.at<float>(i,j))));

            }
        }


       ui->tableWidget_camError->setItem(0,0,new QTableWidgetItem("Error: "+QString("%1").arg(cam_1.calibrationError.at<float>(0,0))));
       ui->tableWidget_camError->setItem(1,0,new QTableWidgetItem("exError: "+QString("%1").arg(cam_1.calibrationError.at<float>(1,0))));




        VirtualCamera cam_2;
        VirtualCamera cam_3;
    }


    //这里目前没有问题，存在img在存储的时候，主线程进行图片的显示
    if((ui->tabWidget->currentIndex() == 3)&&(MainWindow::cameraMode == 2))
    {
       static QLabel *temp[]={ui->label_img_1,ui->label_img_2,ui->label_img_3,
                        ui->label_img_4,ui->label_img_5,ui->label_img_6,
                        ui->label_img_7,ui->label_img_8
                       };
         myMutex.lock();
         QImage disImage;
         QString path="../Pictures/Hardware_trigger_frame/";
         for (int i=0;i<8;i++)
         {
             disImage.load(path+QString::fromStdString(img_name[i]));

             //temp[i]->setPixmap(QPixmap::fromImage(disImage.scaled(temp[i]->size(), Qt::KeepAspectRatioByExpanding)));
             temp[i]->setPixmap(QPixmap::fromImage(disImage));
         }
         myMutex.unlock();
    }

}


void MainWindow::changeEvent(QEvent *event)
{
    if(event->type()!=QEvent::WindowStateChange) return;
    static bool flag=1;
    if((this->windowState()==Qt::WindowMaximized)||(this->windowState()==Qt::WindowFullScreen))
    {
        ui->tableWidget_camDistortion->setColumnWidth(0,100);
        ui->tableWidget_camDistortion->setColumnWidth(1,100);
        ui->tableWidget_camDistortion->setColumnWidth(2,100);
        ui->tableWidget_camDistortion->setColumnWidth(3,100);
        ui->tableWidget_camDistortion->setColumnWidth(4,100);

        ui->tableWidget_camDistortion_2->setColumnWidth(0,100);
        ui->tableWidget_camDistortion_2->setColumnWidth(1,100);
        ui->tableWidget_camDistortion_2->setColumnWidth(2,100);
        ui->tableWidget_camDistortion_2->setColumnWidth(3,100);
        ui->tableWidget_camDistortion_2->setColumnWidth(4,100);

        ui->tableWidget_camDistortion_3->setColumnWidth(0,100);
        ui->tableWidget_camDistortion_3->setColumnWidth(1,100);
        ui->tableWidget_camDistortion_3->setColumnWidth(2,100);
        ui->tableWidget_camDistortion_3->setColumnWidth(3,100);
        ui->tableWidget_camDistortion_3->setColumnWidth(4,100);
        flag=0;
    }

    if((this->windowState()==Qt::WindowMinimized)||(this->windowState()==Qt::WindowNoState))
    {
        ui->tableWidget_camDistortion->setColumnWidth(0,60);
        ui->tableWidget_camDistortion->setColumnWidth(1,60);
        ui->tableWidget_camDistortion->setColumnWidth(2,60);
        ui->tableWidget_camDistortion->setColumnWidth(3,60);
        ui->tableWidget_camDistortion->setColumnWidth(4,60);

        ui->tableWidget_camDistortion_2->setColumnWidth(0,60);
        ui->tableWidget_camDistortion_2->setColumnWidth(1,60);
        ui->tableWidget_camDistortion_2->setColumnWidth(2,60);
        ui->tableWidget_camDistortion_2->setColumnWidth(3,60);
        ui->tableWidget_camDistortion_2->setColumnWidth(4,60);

        ui->tableWidget_camDistortion_3->setColumnWidth(0,60);
        ui->tableWidget_camDistortion_3->setColumnWidth(1,60);
        ui->tableWidget_camDistortion_3->setColumnWidth(2,60);
        ui->tableWidget_camDistortion_3->setColumnWidth(3,60);
        ui->tableWidget_camDistortion_3->setColumnWidth(4,60);
        flag=1;
    }
}






void MainWindow::closeEvent(QCloseEvent *event)
{
    //TODO: 在退出窗口之前，实现希望做的操作

    switch( QMessageBox::information(this, tr("Quit Control View ?"),
                                     tr("Do you really want to quit Control View ?"),
                                     tr("Yes"), tr("No")
                                     ))
        {
            case 0:
                MainWindow::cameraMode = 0;
                MainWindow::thread_mode = 0;
                cv::waitKey(1000);
                event->accept();
                break;
            case 1:
            default:
                event->ignore();
                break;
        }
}


void MainWindow::TestThread_1()
{   
    PylonCamera camera;
    while (MainWindow::thread_mode)
    {
        Sleep(1000);
        switch(MainWindow::cameraMode)
        {
           case 0:
               MainWindow::cameraModeOlder=0;
               qDebug()<<QString::fromStdWString(L"******相机关闭状态*******");
               Sleep(10000);
               break;
           case 1:
              //切换抓取模式初始化
               Sleep(1000);
               qDebug()<<"************************init_grap_mode****************************"<<endl;
               PylonInitialize();
               camera.ptrCamera =new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());
               camera.initCameraGrap();
               MainWindow::cameraModeOlder=1;
               while(MainWindow::cameraMode == 1)
               {
                   qDebug() <<"************************camera_grap_trigger*****************************"<<endl;

                   camera.CameraGrap();
                   //uint8_t *pImageBuffer = (uint8_t *)camera.ptrGrabResult->GetBuffer();
                   //MainWindow::openCvImage=cv::Mat((int)camera.ptrGrabResult->GetHeight(), (int)camera.ptrGrabResult->GetWidth(), CV_8UC1,pImageBuffer);
               }
               myMutex.lock();
               camera.ptrCamera->StopGrabbing();
               PylonTerminate();
               delete  camera.ptrCamera;
               camera.ptrCamera=nullptr;
               myMutex.unlock();
               break;
           case 2:
               Sleep(1000);
               //切换硬件触发模式初始化
               MainWindow::cameraModeOlder=2;
               qDebug()<<"************************init_hardware_trigger****************************"<<endl;
               PylonInitialize();
               camera.ptrCamera =new CInstantCamera(CTlFactory::GetInstance().CreateFirstDevice());
               camera.initGrab_Strategies();
               while(MainWindow::cameraMode == 2)
               {
                   Sleep(100);
                   //myMutex.lock();
                   qDebug() <<"********************camera_hardware_trigger********************"<<endl;
                   camera.Grab_Strategies();
                   //myMutex.unlock();
               }
               //Stop the grabbing.
               myMutex.lock();
               camera.ptrCamera->StopGrabbing();
               PylonTerminate();
               delete  camera.ptrCamera;
               camera.ptrCamera=nullptr;
               myMutex.unlock();

               break;

        case 3:
               qDebug()<<QString::fromStdWString(L"******相机关闭状态*******");
               MainWindow::cameraModeOlder=3;
               break;
           default:
                qDebug() <<"default"<<endl;
                break;
       }
    }
}


void MainWindow::generate3DCloud()
{
    ui->tabWidget->setCurrentIndex(4);
    ui->tabWidget->currentIndex();
}


//****************相机标定，标定相机内参**************
void MainWindow::on_pushButton_cam_intri_clicked()
{

    //开一个新进程，用于相机标定，在标定未完成前，继续按标定按钮无效，标定完，可以继续标定
    if(this->ptrThread_cameraCalibration==nullptr)
    {
        this->ptrThread_cameraCalibration = new std::thread(&MainWindow::cameraCalibration_1_Thread,this,true);
        this->ptrThread_cameraCalibration->detach();
    }
    else
    {
        std::cout<<"*****************正在标定******************************"<<std::endl;
    }
}


//****************相机标定，标定相机外参**************
void MainWindow::on_pushButton_cam_extri_clicked()
{
    //开一个新进程，用于相机标定，在标定未完成前，继续按标定按钮无效，标定完，可以继续标定
    if(this->ptrThread_cameraCalibration==nullptr)
    {
        this->ptrThread_cameraCalibration = new std::thread(&MainWindow::cameraCalibration_1_Thread,this,false);
        this->ptrThread_cameraCalibration->detach();
    }
    else
    {
        std::cout<<"*****************正在标定******************************"<<std::endl;
    }
}


void MainWindow::cameraCalibration_1_Thread(bool isIntri)
{
    CameraCalibration CameraCalib_1;
    CameraCalib_1.loadCalibData("../calibrationData/camera1/output/calib.xml");

    std::vector<std::string> cameraParam(6,"#####");
    CameraCalibration::loadCameraConfig("../calibrationData/camera1/input/camConfig.xml",cameraParam);


    //开启相机标定
    CameraCalib_1.setSquareSize(cv::Size2d(atof(cameraParam[2].c_str()),atof(cameraParam[3].c_str())));
    CameraCalib_1.setCornersSize(cv::Size(atoi(cameraParam[0].c_str()),atoi(cameraParam[1].c_str())));

    if(isIntri)
    {
        //导入所需要的图片到Mat
        CameraCalib_1.loadCameraImgs(QString::fromStdString(cameraParam[4]));

        //标定内参，可以不需要导入外参图片
        //CameraCalib_1.loadCameraExtriImgs(QString::fromStdString(cameraParam[5]));

        CameraCalib_1.extractImageCorners();
        CameraCalib_1.calibrateCamera();
        foreach (double var, CameraCalib_1.err_of_each_calibImgs)
        {
            std::cout<<var<<std::endl;
        }
        //保存内参、畸变参数、内参误差
        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_matrix.txt",CAMCALIB_OUT_MATRIX);
        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_distortion.txt",CAMCALIB_OUT_DISTORTION);
        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_error.txt",CAMCALIB_OUT_ERROR);

        while(1)
        {

            std::cout<<"Esc键不保存标定结果...\nEenter键保存标定结果...\n保存按钮保存标定结果..."<<std::endl;
            if(this->saveFileFlag==1)
            {
                //保存xml文件
                std::cout<<"保存标定结果"<<std::endl;
                CameraCalib_1.saveCalibData("../calibrationData/camera1/output/calib.xml");
                this->saveFileFlag=0;
                break;
            }
            Sleep(2000);

            if(cv::waitKey(10)==27)
            {
                std::cout<<"不保存标定结果"<<std::endl;
                this->saveFileFlag=0;
                break;
            }

            if(cv::waitKey(10)==13)
            {
                std::cout<<"保存标定结果"<<std::endl;
                CameraCalib_1.saveCalibData("../calibrationData/camera1/output/calib.xml");
                this->saveFileFlag=0;
                break;
            }
        }
    }
    else
    {
        //标定外参
        CameraCalib_1.loadCameraExtriImgs(QString::fromStdString(cameraParam[5]));

        CameraCalib_1.findCameraExtrisics();

        //保存外参、外参误差
//        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_matrix.txt",CAMCALIB_OUT_MATRIX);
//        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_distortion.txt",CAMCALIB_OUT_DISTORTION);
        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_error.txt",CAMCALIB_OUT_ERROR);
        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_rotation_matrix.txt",CAMCALIB_OUT_ROTATION);
        CameraCalib_1.exportTxtFiles("../calibrationData/camera1/output/cam_trans_vectror.txt",CAMCALIB_OUT_TRANSLATION);


 //************************************test find homography*********************
        VirtualCamera cam_1;
        cam_1.loadCameraMatrix("../calibrationData/camera1/output/cam_matrix.txt");
        cam_1.loadDistortion("../calibrationData/camera1/output/cam_distortion.txt");
        cam_1.loadRotationMatrix("../calibrationData/camera1/output/cam_rotation_matrix.txt");
        cam_1.loadTranslationVector("../calibrationData/camera1/output/cam_trans_vectror.txt");
        cam_1.computeHomographyMatrix();
        std::cout<<cam_1.homographyMatrix<<std::endl;



        std::vector<cv::Point2f> imgPoints;
        std::vector<cv::Point3f> objPoints3D;

        std::vector<cv::Point3f> calculateObjPoints3D;
        CameraCalib_1.findCornersInCamImg(CameraCalib_1.extrImg, &imgPoints);
        CameraCalib_1.findCornersInObjetBoard(&objPoints3D);

        std::vector<cv::Point2f> objPoints2d(objPoints3D.size());
        std::vector<cv::Point2f> imgcal(objPoints3D.size());

        for(int i=0; i < objPoints3D.size(); i++)
        {
             objPoints2d[i].x=objPoints3D[i].x;
             objPoints2d[i].y=objPoints3D[i].y;
        }


        CameraCalib_1.perspectiveTransformation(objPoints2d,cam_1.homographyMatrix,calculateObjPoints3D);

        int  index =0;
        foreach (cv::Point3f var,calculateObjPoints3D)
        {

            imgcal[index].x=calculateObjPoints3D[index].x;
            imgcal[index].y=calculateObjPoints3D[index].y;
            std::cout<<imgPoints[index]<<std::endl;
            std::cout<<imgcal[index]<<std::endl;
            index++;
        }

      std::cout<< cv::norm(imgcal, imgPoints, cv::NORM_L2)/imgcal.size()<<std::endl;//平方和误差（没有开方）,这个误差也可以叫做重投影误差，这个是评价相机标定结果的一个指标。

      calculateObjPoints3D.clear();
      CameraCalib_1.perspectiveTransformation(imgPoints,cam_1.homographyMatrix.inv(DECOMP_LU),calculateObjPoints3D);
      index =0;
      foreach (cv::Point3f var,calculateObjPoints3D)
      {
          std::cout<<objPoints3D[index]<<std::endl;
          std::cout<<calculateObjPoints3D[index]<<std::endl;
          index++;
      }







       cv::Mat homo=cv::findHomography(objPoints2d,imgPoints);
        std::cout<<cv::findHomography(objPoints2d,imgPoints)<<std::endl;

        calculateObjPoints3D.clear();
        CameraCalib_1.perspectiveTransformation(objPoints2d,homo,calculateObjPoints3D);
        int  tt =0;
        foreach (cv::Point3f var,calculateObjPoints3D)
        {


            imgcal[tt].x=calculateObjPoints3D[tt].x;
            imgcal[tt].y=calculateObjPoints3D[tt].y;
            std::cout<<imgPoints[tt]<<std::endl;
            std::cout<<imgcal[tt]<<std::endl;
            tt++;

        }
  std::cout<< cv::norm(imgcal, imgPoints, cv::NORM_L2)/imgcal.size()<<std::endl;//平方和误差（没有开方）,这个误差也可以叫做重投影误差，这个是评价相机标定结果的一个指标。

  calculateObjPoints3D.clear();
  CameraCalib_1.perspectiveTransformation(imgPoints,homo.inv(DECOMP_LU),calculateObjPoints3D);
  index =0;
  foreach (cv::Point3f var,calculateObjPoints3D)
  {
      std::cout<<objPoints3D[index]<<std::endl;
      std::cout<<calculateObjPoints3D[index]<<std::endl;
      index++;
  }

  //************************************test find homography*********************


        while(1)
        {

            std::cout<<"Esc键不保存标定结果...\nEenter键保存标定结果...\n保存按钮保存标定结果..."<<std::endl;
            if(this->saveFileFlag==1)
            {
                //保存xml文件
                std::cout<<"保存标定结果"<<std::endl;
                CameraCalib_1.saveCalibData("../calibrationData/camera1/output/calib.xml");
                this->saveFileFlag=0;
                break;
            }
            Sleep(3000);

            if(cv::waitKey(10)==27)
            {
                std::cout<<"不保存标定结果"<<std::endl;
                this->saveFileFlag=0;
                break;
            }

            if(cv::waitKey(10)==13)
            {
                std::cout<<"保存标定结果"<<std::endl;
                CameraCalib_1.saveCalibData("../calibrationData/camera1/output/calib.xml");
                this->saveFileFlag=0;
                break;
            }
        }
    }
    delete this->ptrThread_cameraCalibration;
    this->ptrThread_cameraCalibration=nullptr;
}




void MainWindow::on_pushButton_camWriteSetting_clicked()
{
    std::vector<std::string> cameraParam;
    cameraParam.push_back((this->ui->tableWidget_camSetting->item(1,0)->text()).toStdString());
    cameraParam.push_back((this->ui->tableWidget_camSetting->item(1,1)->text()).toStdString());
    cameraParam.push_back((this->ui->tableWidget_camSetting->item(1,2)->text()).toStdString());
    cameraParam.push_back((this->ui->tableWidget_camSetting->item(1,3)->text()).toStdString());
    cameraParam.push_back((this->ui->lineEdit_cam_intri->text()).toStdString());
    cameraParam.push_back((this->ui->lineEdit_cam_extri->text()).toStdString());
    CameraCalibration::saveCameraConfig("../calibrationData/camera1/input/camConfig.xml",cameraParam);
    CameraCalibration::loadCameraConfig("../calibrationData/camera1/input/camConfig.xml",cameraParam);

    for(size_t i = 0; i < cameraParam.size()-2; i++)
    {
         ui->tableWidget_camSetting->setItem(1,i,new QTableWidgetItem(QString::fromStdString(cameraParam[i])));
    }
    ui->lineEdit_cam_intri->setText(QString::fromStdString(cameraParam[4]));
    ui->lineEdit_cam_extri->setText(QString::fromStdString(cameraParam[5]));

}

void MainWindow::on_pushButton_camReadSetting_clicked()
{
    std::vector<std::string> cameraParam(6,"#####");
    CameraCalibration::loadCameraConfig("../calibrationData/camera1/input/camConfig.xml",cameraParam);

    for(size_t i = 0; i < cameraParam.size()-2; i++)
    {
         ui->tableWidget_camSetting->setItem(1,i,new QTableWidgetItem(QString::fromStdString(cameraParam[i])));
    }
    ui->lineEdit_cam_intri->setText(QString::fromStdString(cameraParam[4]));
    ui->lineEdit_cam_extri->setText(QString::fromStdString(cameraParam[5]));
}


void MainWindow::on_toolButton_cam_intri_clicked()
{
    QString file_path = QFileDialog::getExistingDirectory(this,QString::fromStdWString(L"请选择文件夹路径"), "../Pictures/");
    ui->lineEdit_cam_intri->setText(file_path);

}

void MainWindow::on_toolButton_cam_extri_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this,QString::fromStdWString(L"请选择外参文件路径"), "../Pictures/Grap_frame/");
    ui->lineEdit_cam_extri->setText(file_path);

}



void MainWindow::on_pushButton_cam_save_clicked()
{

    this->saveFileFlag=1;
}


