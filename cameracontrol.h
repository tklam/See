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

#ifndef CAMERACONTROL_H
#define CAMERACONTROL_H

#include <QObject>
#include <QCamera>
#include <QCameraImageCapture>

class CameraControl : public QObject
{
    Q_OBJECT
public:
    explicit CameraControl(QObject *parent = 0);
    ~CameraControl();

signals:
    void savedImageCaptured(const QString & filename);

public slots:
    void onSavedImageCaptured(int id, const QString & filename);

public:
    void setupCamera(QCamera* camera);
    void startCamera();
    void searchAndLock();
    void captureToLocation(QString path);
    void unlock(QCamera::LockTypes lockTypes);

private:
    QCamera * camera;
    QCameraImageCapture * imageCapture;
};

#endif // CAMERACONTROL_H
