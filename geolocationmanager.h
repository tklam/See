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

#ifndef GEOLOCATIONMANAGER_H
#define GEOLOCATIONMANAGER_H

#include <QObject>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>

class GeolocationManager : public QObject
{
    Q_OBJECT
public:
    explicit GeolocationManager(QObject *parent = 0);
    ~GeolocationManager();
    void startUpdates();
    void stopUpdates();
    QGeoPositionInfo const & getLastGeoPositionInfo() const;

signals:
    void longitudeUpdated(double coor);
    void latitudeUpdated(double coor);

public slots:
private slots:
    void positionUpdated(const QGeoPositionInfo &info);
private:
    QGeoPositionInfoSource *geolocationSource;
    QGeoPositionInfo lastGeoPosition;
};

#endif // GEOLOCATIONMANAGER_H
