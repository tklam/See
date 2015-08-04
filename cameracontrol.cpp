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

#include "cameracontrol.h"

#include <QCameraImageProcessing>

CameraControl::CameraControl(QObject *parent) : QObject(parent)
{
}

CameraControl::~CameraControl()
{
    //camera->stop();
    //camera->unload();

    delete imageCapture;
    camera->unlock();
}

void CameraControl::setupCamera(QCamera * camera)
{
    this->camera = camera;

    QCameraFocus * focus = camera->focus();
    focus->setFocusMode(QCameraFocus::ContinuousFocus);
    focus->setFocusPointMode(QCameraFocus::FocusPointAuto);

    camera->imageProcessing()->setWhiteBalanceMode(QCameraImageProcessing::WhiteBalanceAuto);
    camera->setCaptureMode(QCamera::CaptureViewfinder);

    QCameraExposure * exposure = camera->exposure();
    exposure->setExposureMode(QCameraExposure::ExposureAuto);
    exposure->setExposureCompensation(0);
    exposure->setMeteringMode(QCameraExposure::MeteringSpot);

    imageCapture = new QCameraImageCapture(camera, this);

    connect(imageCapture, SIGNAL(imageSaved(int, QString)), this, SLOT(onSavedImageCaptured(int, QString)));

    //camera->load();
    //camera->start();
}

void CameraControl::startCamera()
{
    camera->start();
}

void CameraControl::searchAndLock()
{
    camera->searchAndLock();
}

void CameraControl::captureToLocation(QString path)
{
    imageCapture->capture(path);
}

void CameraControl::unlock(QCamera::LockTypes lockTypes)
{
    camera->unlock(lockTypes);
}

void CameraControl::onSavedImageCaptured(int id, const QString & filename)
{
    emit savedImageCaptured(filename);
}
