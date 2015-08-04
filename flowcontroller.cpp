/*

Copyright (C) 2015 LAM Tak-Kei

This file is part of See.

See is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

See is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
See. If not, see <http://www.gnu.org/licenses/>.

*/

#include "flowcontroller.h"
#include <QFile>

FlowController::FlowController(QObject *parent) :
    QObject(parent),
    tmpImageCaptureFilename(".tmpColourIdentifer"),
    workingMode(FlowController::CALIBRATE_WHITE),
    isProcessingImage(false),
    futureWatcher(NULL)
{
}

FlowController::~FlowController()
{
    delete futureWatcher;
}

void FlowController::onCalibrateWhite()
{
    if(isProcessingImage) {
        return;
    }
    isProcessingImage = true;

    workingMode = FlowController::CALIBRATE_WHITE;
    cameraControl->searchAndLock();
    cameraControl->captureToLocation(tmpImageCaptureFilename);
}

void FlowController::onDetectColour()
{
    if(isProcessingImage) {
        return;
    }
    isProcessingImage = true;

    workingMode = FlowController::DETECT_COLOUR;
    cameraControl->searchAndLock();
    cameraControl->captureToLocation(tmpImageCaptureFilename);
}

void FlowController::onSavedImageCapured(const QString & filename)
{
    capturedImageFilename = filename;

    switch(workingMode) {
    case CALIBRATE_WHITE:
        cameraControl->unlock(QCamera::LockExposure | QCamera::LockFocus);
        break;
    case DETECT_COLOUR:
        cameraControl->unlock(QCamera::LockExposure | QCamera::LockFocus);
        break;
    }

    analyseImage(filename);
}

void FlowController::analyseImage(const QString & filename)
{
    imageAnalysisStatus = "analyzing...";
    emit updateImageAnalysisStatus(imageAnalysisStatus);

    delete futureWatcher;
    futureWatcher = new QFutureWatcher<void>();

    switch(workingMode) {
    case CALIBRATE_WHITE:
        connect(futureWatcher, SIGNAL(finished()), this, SLOT(onFinishedCalibrateWhite()));
        future = QtConcurrent::run(imageProcessor, &ImageProcessor::calibrateWhite, filename);
        break;
    case DETECT_COLOUR:
        connect(futureWatcher, SIGNAL(finished()), this, SLOT(onFinishedDetectColour()));
        future = QtConcurrent::run(imageProcessor, &ImageProcessor::detectColour, filename);
        break;
    }

    futureWatcher->setFuture(future);
}

void FlowController::onFinishedCalibrateWhite()
{
    RGB rgb;
    imageProcessor->getAnalysedRgbValue(rgb);
    imageAnalysisStatus = formatRgb(rgb);
    emit updateImageAnalysisStatus(imageAnalysisStatus);
    emit updateIdentifiedColour(QColor(rgb.red, rgb.green, rgb.blue));

    QFile::remove(capturedImageFilename);
    isProcessingImage = false;
}

void FlowController::onFinishedDetectColour()
{
    RGB rgb;
    imageProcessor->getAnalysedRgbValue(rgb);
    imageAnalysisStatus = formatRgb(rgb);
    emit updateImageAnalysisStatus(imageAnalysisStatus);
    emit updateIdentifiedColour(QColor(rgb.red, rgb.green, rgb.blue));

    QFile::remove(capturedImageFilename);
    isProcessingImage = false;
}

void FlowController::setCameraControl(CameraControl & cameraControl)
{
    this->cameraControl = &cameraControl;
    connect(this->cameraControl, SIGNAL(savedImageCaptured(QString)), this, SLOT(onSavedImageCapured(QString)));
}

void FlowController::setImageProcessor(ImageProcessor & imageProcessor)
{
    this->imageProcessor = &imageProcessor;
}

QString FlowController::formatRgb(RGB const & rgb)
{
    QString rgbString;
    rgbString.sprintf("(%d, %d, %d)", rgb.red, rgb.blue, rgb.green);
    return rgbString;
}
