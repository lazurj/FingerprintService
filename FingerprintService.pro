#-------------------------------------------------
#
# Project created by QtCreator 2019-03-23T14:21:54
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FingerPrintService
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    service.cpp \
    server.cpp \
    clientworker.cpp

HEADERS += \
    service.h \
    server.h \
    clientworker.h \
    enums.h \
    config.h \
    ui_service.h

FORMS += \
        service.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += /usr/local/include

#CUDA
unix:!macx: LIBS += -L/opt/cuda/lib64/ -lcudart
unix:!macx: LIBS += -L/usr/local/cuda/lib64/ -lcudart
INCLUDEPATH += /opt/cuda/include
DEPENDPATH += /opt/cuda/include
INCLUDEPATH += /usr/local/cuda/include
DEPENDPATH += /usr/local/cuda/include

#ArrayFire
unix:!macx: LIBS += -L/usr/lib/ -lafcuda
unix:!macx: LIBS += -L/usr/local/lib/ -lafcuda
unix:!macx: LIBS += -L/usr/local/lib64/ -lafcuda

#Caffe
unix:!macx: LIBS += -L/usr/local/lib64/ -lcaffe
unix:!macx: LIBS += -L/usr/local/lib/ -lcaffe
unix:!macx: LIBS += -L/usr/lib/ -lboost_system
unix:!macx: LIBS += -L/usr/lib/ -lglog
unix:!macx: LIBS += -L/usr/lib/ -lprotobuf


#OpenCV
INCLUDEPATH += /usr/include/opencv4

LIBS += -L/usr/local/lib -lopencv_core -lopencv_imgcodecs
# -lopencv_highgui
#Preprocessing
unix:!macx: LIBS += -L$$PWD/../Preprocessing/ -lPreprocessing
INCLUDEPATH += $$PWD/../Preprocessing
DEPENDPATH += $$PWD/../Preprocessing

#Matcher
unix:!macx: LIBS += -L$$PWD/../Matcher/ -lMatcher
INCLUDEPATH += $$PWD/../Matcher
DEPENDPATH += $$PWD/../Matcher

#Extraction
unix:!macx: LIBS += -L$$PWD/../Extraction/ -lExtraction
INCLUDEPATH += $$PWD/../Extraction
DEPENDPATH += $$PWD/../Extraction


INCLUDEPATH +=/home/jakub/Desktop/BioMiniSDK_for_Linux_v3.6.0.1320/include
DEPENDPATH +=/home/jakub/Desktop/BioMiniSDK_for_Linux_v3.6.0.1320/include

unix:!macx: LIBS += -L/home/jakub/Desktop/BioMiniSDK_for_Linux_v3.6.0.1320/bin/x64 -lUFMatcher

DISTFILES += \
    core/config/Caffe/extraction.caffemodel \
    core/config/Caffe/frequency.caffemodel \
    core/config/Caffe/mask.caffemodel \
    core/config/Caffe/extraction_imagemean.binaryproto \
    core/config/Caffe/frequency_imagemean.binaryproto \
    core/config/Caffe/mask_imagemean.binaryproto \
    core/config/Caffe/extraction_deploy.prototxt \
    core/config/Caffe/extraction_labels.txt \
    core/config/Caffe/frequency_deploy.prototxt \
    core/config/Caffe/frequency_labels.txt \
    core/config/Caffe/mask_deploy.prototxt \
    core/config/Caffe/mask_labels.txt
