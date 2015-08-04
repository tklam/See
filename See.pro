TEMPLATE = app

CONFIG += c++11
QT += qml quick widgets multimedia multimediawidgets positioning

TARGET=See

SOURCES += main.cpp \
    presentationpropertyholder.cpp \
    geolocationmanager.cpp \
    imageprocessor.cpp \
    cameracontrol.cpp \
    flowcontroller.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    presentationpropertyholder.h \
    geolocationmanager.h \
    imageprocessor.h \
    cameracontrol.h \
    flowcontroller.h


INCLUDEPATH += "$$PWD/../OpenCV-android-sdk/sdk/native/jni/include"
android {
    LIBS += \
        -L"$$_PRO_FILE_PWD_/../OpenCV-android-sdk/sdk/native/3rdparty/libs/armeabi-v7a"\
        -L"$$_PRO_FILE_PWD_/../OpenCV-android-sdk/sdk/native/libs/armeabi-v7a"\
        -llibtiff\
        -llibjpeg\
        -llibjasper\
        -llibpng\
        -lIlmImf\
        -ltbb\
        -lopencv_core\
        -lopencv_androidcamera\
        -lopencv_flann\
        -lopencv_imgproc\
        -lopencv_highgui\
        -lopencv_features2d\
        -lopencv_calib3d\
        -lopencv_ml\
        -lopencv_objdetect\
        -lopencv_video\
        -lopencv_contrib\
        -lopencv_photo\
        -lopencv_java\
        -lopencv_legacy\
        -lopencv_ocl\
        -lopencv_stitching\
        -lopencv_superres\
        -lopencv_ts\
        -lopencv_videostab

 #Enable automatic NEON vectorization
    QMAKE_CXXFLAGS -= -mfpu=vfp
    QMAKE_CXXFLAGS_RELEASE -= -mfpu=vfp
    QMAKE_CXXFLAGS += -mfpu=neon -ftree-vectorize -ftree-vectorizer-verbose=1 -mfloat-abi=softfp
    QMAKE_CXXFLAGS_RELEASE += -mfpu=neon -ftree-vectorize -ftree-vectorizer-verbose=1 -mfloat-abi=softfp

    ANDROID_PACKAGE_SOURCE_DIR=$$_PRO_FILE_PWD_/android
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle
