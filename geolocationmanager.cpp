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

#include "geolocationmanager.h"
#include <QDebug>

GeolocationManager::GeolocationManager(QObject *parent) : QObject(parent)
{
    geolocationSource = QGeoPositionInfoSource::createDefaultSource(this);
    //qDebug() << "geolocationSource: " << geolocationSource;
    if(geolocationSource) {
        geolocationSource->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
        geolocationSource->setUpdateInterval(10000);
        geolocationSource->setProperty("stationaryDetectionEnabled", true);
        connect(geolocationSource, SIGNAL(positionUpdated(QGeoPositionInfo)),
                this, SLOT(positionUpdated(QGeoPositionInfo)));
    }

}

GeolocationManager::~GeolocationManager()
{
    if(geolocationSource) {
        geolocationSource->stopUpdates();
        delete geolocationSource;
    }
}

void GeolocationManager::positionUpdated(const QGeoPositionInfo &info)
{
    //qDebug() << "Last position updated:" << info;

    lastGeoPosition = info;
    emit longitudeUpdated(info.coordinate().longitude());
    emit latitudeUpdated(info.coordinate().latitude());
}

void GeolocationManager::startUpdates()
{
    if(geolocationSource) {
        geolocationSource->startUpdates();
    }
}

void GeolocationManager::stopUpdates()
{
    if(geolocationSource) {
        geolocationSource->stopUpdates();
    }
}

QGeoPositionInfo const & GeolocationManager::getLastGeoPositionInfo() const
{
    return lastGeoPosition;
}
