QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DialogCalibration.cpp \
    DialogCamera.cpp \
    DialogDetector.cpp \
    DialogExtrinsic.cpp \
    DialogIntrinsic.cpp \
    DialogRobot.cpp \
    DialogStart.cpp \
    ExtrinsicCalib.cpp \
    Logger.cpp \
    PathPlanning.cpp \
    Scheduler.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    DialogCalibration.h \
    DialogCamera.h \
    DialogDetector.h \
    DialogExtrinsic.h \
    DialogIntrinsic.h \
    DialogRobot.h \
    DialogStart.h \
    ExtrinsicCalib.h \
    GlobalHeader.h \
    Logger.h \
    PathPlanning.h \
    Scheduler.h \
    mainwindow.h

FORMS += \
    DialogCalibration.ui \
    DialogCamera.ui \
    DialogExtrinsic.ui \
    DialogIntrinsic.ui \
    DialogRobot.ui \
    DialogStart.ui \
    mainwindow.ui

# OpenCV
OPENCV_PATH = /usr/local
INCLUDEPATH += /usr/include/eigen3
INCLUDEPATH += $${OPENCV_PATH}/include/opencv4
LIBS += -L$${OPENCV_PATH}/lib \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_highgui \
        -lopencv_dnn \
        -lopencv_calib3d

# If pkg-config is available and opencv4.pc is found
system(pkg-config --exists opencv4) {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv4
}

# RealSense
LIBS += -lrealsense2

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
