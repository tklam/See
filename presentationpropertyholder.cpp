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

#include "presentationpropertyholder.h"

PresentationPropertyHolder::PresentationPropertyHolder()
{
}

PresentationPropertyHolder::~PresentationPropertyHolder()
{

}

QString PresentationPropertyHolder::content() const
{
    return _content;
}

void PresentationPropertyHolder::setContent(QString const & content)
{
    _content = content;
    emit contentChanged();
}

void PresentationPropertyHolder::onUpdateContent(QString const & content)
{
    setContent(content);
}

void PresentationPropertyHolder::onUpdateContent(float content)
{
    setContent(QString::number(content));
}

void PresentationPropertyHolder::onUpdateContent(double content)
{
    setContent(QString::number(content));
}

void PresentationPropertyHolder::onUpdateContent(QColor content)
{
    setContent(content.name());
}
