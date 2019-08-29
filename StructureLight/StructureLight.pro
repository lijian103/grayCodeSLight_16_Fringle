#-------------------------------------------------
#
# Project created by QtCreator 2019-06-14T22:35:26
#
#-------------------------------------------------

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = StructureLight
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
CONFIG += c++11
RC_ICONS = $$PWD/../../../Docs/ueg.ico
##OpenCV lib and include
#INCLUDEPATH +=$$PWD/../../../Libs/OpenCV/include
#INCLUDEPATH +=$$PWD/../../../Libs/OpenCV/include/opencv
#INCLUDEPATH +=$$PWD/../../../Libs/OpenCV/include/opencv2
#LIBS += $$PWD/../../../Libs/OpenCV/x64/vc15/lib/opencv_world410d.lib
##LIBS += $$PWD/../../../Libs/OpenCV/x64/vc15/lib/opencv_world410.lib

##PylonCamera lib and include
#INCLUDEPATH +=$$PWD/../../../Libs/PylonCamera/include
##Ìí¼Ó¿âÄ¿Â¼
##LIBS += D:/Trunk/Libs/PylonCamera/lib/x64
##LIBS += -LD:/Trunk/Libs/PylonCamera/lib/x64
#LIBS += -L$$PWD/../../../Libs/PylonCamera/lib/x64

#***************python config ************
INCLUDEPATH +=C:\Users\tommy\Anaconda3\include   ############# python enviroment
LIBS += -LC:\Users\tommy\Anaconda3\libs\
-l_tkinter\
-lpython3\
-lpython37

DISTFILES += \
             ./toolSource/python_to_cplusplus/hell.py

SOURCES += \
        main.cpp \
        mainwindow.cpp\
        Source/camera.cpp \
        toolSource/timeMeasurement/time_measurement.cpp \
        toolSource/structured_light/utilities.cpp \
        toolSource/structured_light/camera_calibration.cpp \
        toolSource/structured_light/virtualCamera.cpp \
        toolSource/python_to_cplusplus/python_to_cplusplus.cpp \
        softEntry.cpp \
        toolSource/structured_light/projector_calibration.cpp\
        toolSource/structured_light/PointCloudImage.cpp\
        toolSource/structured_light/MeshCreator.cpp\
        toolSource/structured_light/GrayCodes.cpp \
    toolSource/structured_light/reconstructor.cpp

HEADERS += \
        mainwindow.h \
        Inc/Camera.h \
        toolSource/timeMeasurement/time_measurement.h \
        toolSource/structured_light/utilities.h \
        toolSource/structured_light/camera_calibration.h \
        toolSource/structured_light/virtualCamera.h \
    toolSource/python_to_cplusplus/python_to_cplusplus.h \
    softEntry.h \
    toolSource/structured_light/projector_calibration.h\
        toolSource/structured_light/PointCloudImage.h\
        toolSource/structured_light/MeshCreator.h\
        toolSource/structured_light/GrayCodes.h \
    toolSource/structured_light/reconstructor.h


FORMS += \
    mainwindow.ui


#OpenCV lib and include
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../Libs/OpenCV/x64/vc15/lib/ -lopencv_world410d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../Libs/OpenCV/x64/vc15/lib/ -lopencv_world410d

INCLUDEPATH += $$PWD/../../../Libs/OpenCV/include
INCLUDEPATH += $$PWD/../../../Libs/OpenCV/include/opencv
DEPENDPATH += $$PWD/../../../Libs/OpenCV/include/opencv2

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Libs/OpenCV/x64/vc15/lib/libopencv_world410.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Libs/OpenCV/x64/vc15/lib/libopencv_world410d.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../Libs/OpenCV/x64/vc15/lib/opencv_world410.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../Libs/OpenCV/x64/vc15/lib/opencv_world410d.lib

##PylonCamera lib and include
win32: LIBS += -L$$PWD/../../../Libs/PylonCamera/lib/x64
INCLUDEPATH += $$PWD/../../../Libs/PylonCamera/include
DEPENDPATH += $$PWD/../../../Libs/PylonCamera/include
INCLUDEPATH += $$PWD/PylonCamera
#DEPENDPATH += $$PWD/PylonCamera

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../Libs/PylonCamera/lib/x64
else:win32-g++: PRE_TARGETDEPS += $$PWD/../../../Libs/PylonCamera/lib/x64

RESOURCES += \
    logo.qrc
