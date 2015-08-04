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

#ifndef FLOWCONTROLLER_H
#define FLOWCONTROLLER_H

#include <QObject>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>

#include "cameracontrol.h"
#include "imageprocessor.h"

class FlowController : public QObject
{
    Q_OBJECT
public:
    explicit FlowController(QObject *parent = 0);
    ~FlowController();

    void setCameraControl(CameraControl & cameraControl);
    void setImageProcessor(ImageProcessor &imageProcessor);

    enum Mode {
        CALIBRATE_WHITE = 0,
        DETECT_COLOUR = 1
    };

signals:
    void updateImageAnalysisStatus(QString rgbString);
    void updateIdentifiedColour(QColor);

public slots:
    void onDetectColour();
    void onCalibrateWhite();
    void onSavedImageCapured(const QString & filename);
    void onFinishedDetectColour();
    void onFinishedCalibrateWhite();

private:
    CameraControl * cameraControl;
    ImageProcessor* imageProcessor;
    QString tmpImageCaptureFilename;
    QString capturedImageFilename;
    Mode workingMode;
    bool isProcessingImage;
    QString imageAnalysisStatus;
    QFuture<void> future;
    QFutureWatcher<void> * futureWatcher;

    void analyseImage(const QString & filename);
    QString formatRgb(RGB const & rgb);
};

#endif // FLOWCONTROLLER_H
