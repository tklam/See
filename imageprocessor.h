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

#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <QDebug>
#include <cmath>
#include <QColor>

struct RGB {
    uchar blue;
    uchar green;
    uchar red;
};

// a list of pixels whose sum of RGB value is equal to rgbSum
class RgbSumPixels
{
public:
    size_t rgbSum;

    void addPixel(RGB* rgb)
    {
        rgbs.push_back(rgb);
    }

    void removeAtBack()
    {
        rgbs.pop_back();
    }

    size_t getPixelsCount()
    {
        return rgbs.size();
    }

    const std::list<RGB*> & getPixels() const
    {
        return rgbs;
    }

private:
    std::list<RGB *> rgbs;
};

class ImageProcessor : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcessor(QObject *parent = 0);
    ~ImageProcessor();
    void getAnalysedRgbValue(RGB & rgb) const;

signals:
    //void finishedDetectColour();
    //void finishedCalibrateWhite();

public slots:
    void calibrateWhite(QString imagePath);
    void detectColour(QString imagePath);


private:
    /*
    float cv_distance(cv::Point2f P, cv::Point2f Q);                    // Get Distance between two points
    float cv_lineEquation(cv::Point2f L, cv::Point2f M, cv::Point2f J);     // Perpendicular Distance of a Point J from line formed by Points L and M; Solution to equation of the line Val = ax+by+c
    float cv_lineSlope(cv::Point2f L, cv::Point2f M, int& alignement);  // Slope of a line by two Points L and M on it; Slope of line, S = (x1 -x2) / (y1- y2)
    void cv_getVertices(std::vector<std::vector<cv::Point> > contours, int c_id, float slope, std::vector<cv::Point2f>& X);
    void cv_updateCorner(cv::Point2f P, cv::Point2f ref , float& baseline,  cv::Point2f& corner);
    void cv_updateCornerOr(unsigned char orientation, std::vector<cv::Point2f> IN, std::vector<cv::Point2f> &OUT);
    bool getIntersectionPoint(cv::Point2f a1, cv::Point2f a2, cv::Point2f b1, cv::Point2f b2, cv::Point2f& intersection);
    float cross(cv::Point2f v1, cv::Point2f v2);
    */

    void capValueTo255(float raw, unsigned char & value);
    QString formatRgb(float r, float g, float b);
    void calcAverageForTopPercentageBrightestColour(
        cv::Mat image, const size_t rows, const size_t cols, const size_t maxCount, const uchar lowestColourValue,
        float & average_r, float & average_g, float & average_b);

    const size_t CV_QR_NORTH = 0;
    const size_t CV_QR_EAST = 1;
    const size_t CV_QR_SOUTH = 2;
    const size_t CV_QR_WEST = 3;

    float average_r;
    float average_g;
    float average_b;

    float whiteGain_r;
    float whiteGain_g;
    float whiteGain_b;

    QString processingResult;
};

#endif // IMAGEPROCESSOR_H
