#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QDebug>
#include <thread>
#include <mutex>
// 加载OpenCV API
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>


#include "toolSource/structured_light/camera_calibration.h"

// Namespace for using opencv.


extern std::mutex myMutex;//线程锁
extern int grapFlag;//抓取标志
using namespace std;
namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void TestThread_1();
    void cameraCalibration_1_Thread(bool isIntri);
    void projectorCalibration_1_Thread(bool isIntri);
    static int cameraMode;
    static int cameraModeOlder ;
    static int thread_mode;
    static cv::Mat openCvImage;

private:
    Ui::MainWindow *ui;
    cv::VideoCapture videoCap;
    QButtonGroup *groupButton1;
    std::thread *new_thread;
    std::thread *ptrThread_cameraCalibration=nullptr;


    bool cameraParamReadble=true;
    bool saveFileFlag=0;



protected:
     void closeEvent(QCloseEvent *event);
     void changeEvent(QEvent * event);

private slots:
    void timerUpDate();
    void showHardwarePictures();
    void cameraModeChoose();
    void grapPictures();
    void saveFourPictures();
    void selectDirPathDialg();
    void selectDirPathDialg_grapMode();
    void showGrapPictures();
    void checkBuffersInQueue_total();
    void getStartOrOff_UsbCamera(int state);
    void generate3DCloud();
    void on_pushButton_cam_intri_clicked();
    void on_pushButton_camWriteSetting_clicked();
    void on_pushButton_camReadSetting_clicked();
    void on_toolButton_cam_intri_clicked();
    void on_toolButton_cam_extri_clicked();

    void on_pushButton_cam_save_clicked();
    void on_pushButton_cam_extri_clicked();
    void on_pushButton_camReadSetting_3_clicked();
    void on_pushButton_camWriteSetting_3_clicked();
    void on_pushButton_camReadSetting_2_clicked();
    void on_toolButton_cam_intri_3_clicked();
    void on_toolButton_projector_homo_dir_clicked();
    void on_toolButton_projector_imgPath_clicked();
    void on_toolButton_cam_extri_3_clicked();
    void on_pushButton_cam_intri_3_clicked();
    void on_pushButton_cam_extri_3_clicked();
    void on_pushButton_cam_save_3_clicked();
};


#endif // MAINWINDOW_H
