QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/cuda-10.2/targets/aarch64-linux/include
LIBS += -L/usr/local/cuda-10.2/targets/aarch64-linux/lib/ -lcublas -lcudart -lcudnn -lcufft -lcurand -lcuda

INCLUDEPATH += /usr/include/opencv4
LIBS += -L/usr/lib/aarch64-linux-gnu/ -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio -lopencv_dnn -lopencv_highgui

INCLUDEPATH += /home/oscar/Yolo/darknet/include
LIBS += -L/home/oscar/Yolo/darknet/ -ldarknet

HEADERS += \
    src/darknet.hpp \
    src/detection.hpp \
    src/detector.hpp \
    src/image.hpp \
    src/logging.hpp \
    src/utils.hpp \
    src/convert_cv.hpp \
    src/convert_cv_bgr8.hpp

SOURCES += main.cpp \
    src/convert_cv_bgr8.cpp \
    src/detector.cpp \
    src/image.cpp \
    src/utils.cpp

