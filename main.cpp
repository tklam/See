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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QDebug>
#include <QtQml>
#include <QSslSocket>

#include "geolocationmanager.h"
#include "presentationpropertyholder.h"
#include "imageprocessor.h"
#include "cameracontrol.h"
#include "flowcontroller.h"

int main(int argc, char *argv[])
{
    //--------------------------------------------------------------------- register QML types
    qmlRegisterType<PresentationPropertyHolder>("HKer", 1, 0, "PresentationPropertyHolder");

    QGuiApplication app(argc, argv);
    GeolocationManager geolocationManager;
    ImageProcessor imageProcessor;
    CameraControl cameraControl;
    FlowController flowController;
    flowController.setCameraControl(cameraControl);
    flowController.setImageProcessor(imageProcessor);
    QQmlApplicationEngine engine;

    //----------------------------------------------------------------------------------------- associate context properties
    //engine.rootContext()->setContextProperty("camera", camera.property("mediaObject"));

    //----------------------------------------------------------------------------------------- setup qml
    qputenv("QT_QUICK_CONTROLS_STYLE", "Flat");
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    //----------------------------------------------------------------------------------------- connect signals and slots
    QObject* viewRoot = engine.rootObjects().first();

    PresentationPropertyHolder* longitudeHolder = viewRoot->findChild<PresentationPropertyHolder*>("longitudeHolder");
    QObject::connect(&geolocationManager, SIGNAL(longitudeUpdated(double)), longitudeHolder, SLOT(onUpdateContent(double)));

    PresentationPropertyHolder* latitudeHolder = viewRoot->findChild<PresentationPropertyHolder*>("latitudeHolder");
    QObject::connect(&geolocationManager, SIGNAL(latitudeUpdated(double)), latitudeHolder, SLOT(onUpdateContent(double)));

    PresentationPropertyHolder* colourTextHolder = viewRoot->findChild<PresentationPropertyHolder*>("colourTextHolder");
    QObject::connect(&flowController, SIGNAL(updateImageAnalysisStatus(QString)), colourTextHolder, SLOT(onUpdateContent(QString)));

    PresentationPropertyHolder* colourValueHolder = viewRoot->findChild<PresentationPropertyHolder*>("colourValueHolder");
    QObject::connect(&flowController, SIGNAL(updateIdentifiedColour(QColor)), colourValueHolder, SLOT(onUpdateContent(QColor)));

    QObject::connect(viewRoot, SIGNAL(detectColourClicked()), &flowController, SLOT(onDetectColour()));
    QObject::connect(viewRoot, SIGNAL(calibrateWhiteClicked()), &flowController, SLOT(onCalibrateWhite()));

    //----------------------------------------------------------------- setup camera
    QObject* cameraObj = viewRoot->findChild<QObject*>("camera");
    QVariant cameraMediaObject = cameraObj->property("mediaObject");
    QCamera* camera = qvariant_cast<QCamera*>(cameraMediaObject);
    cameraControl.setupCamera(camera);

    //----------------------------------------------- start the jobs
    geolocationManager.startUpdates();

    return app.exec();
}
