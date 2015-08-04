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

#ifndef PRESENATIONPROPERTYHOLDER_H
#define PRESENATIONPROPERTYHOLDER_H

#include <QQuickItem>

class PresentationPropertyHolder : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)
public:
    PresentationPropertyHolder();
    ~PresentationPropertyHolder();
    QString content() const;
    void setContent(QString const & content);

signals:
    void contentChanged();

public slots:
    void onUpdateContent(QString const & content);
    void onUpdateContent(float content);
    void onUpdateContent(double content);
    void onUpdateContent(QColor content);

public:

private:
    QString _content;
};

#endif // PRESENATIONPROPERTYHOLDER_H
