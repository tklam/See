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

#include "imageprocessor.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sstream>
#include <deque>

using namespace cv;
using namespace std;

void ImageProcessor::capValueTo255(float raw, unsigned char & value)
{
    if(raw > (unsigned char)255) {
        value = 255;
    }
    else {
        value = raw;
    }
}

QString ImageProcessor::formatRgb(float r, float g, float b)
{
    QString rgb;
    rgb.sprintf("(%.1f, %.1f, %.1f)", r, g, b);
    return rgb;
}

static void RemoveExcessAtBack(
    deque<RgbSumPixels*> & topRgbSums,
    size_t & effectivePixelCount,
    const size_t maxCount
)
{
    while(topRgbSums.empty() == false &&
          effectivePixelCount > maxCount) {
        RgbSumPixels * rgbSumPixels = *topRgbSums.rbegin();
        while(rgbSumPixels->getPixelsCount() > 0 &&
              effectivePixelCount > maxCount) {
            rgbSumPixels->removeAtBack();
            --effectivePixelCount;
        }

        if(rgbSumPixels->getPixelsCount() == 0) {
            delete rgbSumPixels;
            topRgbSums.pop_back();
        }
    }
}


static size_t FindJustSmallerOrEqualTo(
    deque<RgbSumPixels*> const & topRgbSums,
    size_t rgbSum)
{

    size_t beginIndex = 0;
    size_t endIndexPlusOne = topRgbSums.size();

    while(beginIndex != endIndexPlusOne) {
        int midIndex = (beginIndex + endIndexPlusOne) / 2;
        if(topRgbSums[midIndex]->rgbSum > rgbSum) {  //[midIndex+1, endIndexPlusOne)
            beginIndex = midIndex + 1;
        }
        else { //[beginIndex, midIndex)
            endIndexPlusOne = midIndex;
        }
    }
    return beginIndex;
}

void ImageProcessor::calcAverageForTopPercentageBrightestColour(
    Mat image, const size_t rows, const size_t cols, const size_t maxCount, const uchar lowestColourValue,
    float & average_r, float & average_g, float & average_b)
{
    deque<RgbSumPixels*> topRgbSums;
    size_t effectivePixelCount = 0;

    for(size_t r = 0; r < rows; ++r) {
        for(size_t c = 0; c < cols; ++c) {
            RGB& rgb = image.ptr<RGB>(r)[c];

            // assume the value for each channel must be greater than whiteThreshold for it to be white
            if(rgb.blue < lowestColourValue &&
               rgb.green < lowestColourValue &&
               rgb.red < lowestColourValue) {
                continue;
            }

            size_t rgbSum = rgb.blue + rgb.green + rgb.red;

            // add the pixel to the list sorted in the decreasing order of rgb sum
            size_t insertIndex = FindJustSmallerOrEqualTo(topRgbSums, rgbSum);

            //qDebug() << "rgbSum: " << rgbSum << " topRgbSums.size(): " << topRgbSums.size() << " insertIndex: " << insertIndex;

            if(insertIndex == topRgbSums.size()) {
                RgbSumPixels * rgbSumPixels = new RgbSumPixels();
                rgbSumPixels->rgbSum = rgbSum;
                rgbSumPixels->addPixel(&rgb);
                topRgbSums.push_back(rgbSumPixels);
            }
            else {
                RgbSumPixels * toCompare = topRgbSums[insertIndex];

                if(rgbSum == toCompare->rgbSum) {
                    toCompare->addPixel(&rgb);
                }
                else {
                    RgbSumPixels * rgbSumPixels = new RgbSumPixels();
                    rgbSumPixels->rgbSum = rgbSum;
                    rgbSumPixels->addPixel(&rgb);
                    topRgbSums.insert(topRgbSums.begin() + insertIndex, rgbSumPixels);
                }
            }

            ++effectivePixelCount;

            // remove excess pixels
            if(effectivePixelCount <= maxCount) {
                continue;
            }
            //else
            RemoveExcessAtBack(
                topRgbSums,
                effectivePixelCount,
                maxCount
            );

        }
    }

    qDebug() << "maxCount: " << maxCount;
    qDebug() << "topRgbSums.size(): " << topRgbSums.size();

    if(topRgbSums.size() == 0) {
        return;
    }

    average_r = 0;
    average_g = 0;
    average_b = 0;
    for(deque<RgbSumPixels*>::const_iterator it = topRgbSums.cbegin(); it != topRgbSums.cend(); ++it) {
        RgbSumPixels * rgbSumPixels = *it;
        //qDebug() << rgbSumPixels->rgbSum;
        const list<RGB*> & pixels = rgbSumPixels->getPixels();
        for(list<RGB*>::const_iterator p_it = pixels.cbegin(); p_it != pixels.cend(); ++p_it) {
            const RGB * rgb = *p_it;
            average_r += rgb->red;
            average_g += rgb->green;
            average_b += rgb->blue;
        }
        delete rgbSumPixels;
    }
    average_r = average_r / effectivePixelCount;
    average_g = average_g / effectivePixelCount;
    average_b = average_b / effectivePixelCount;
}

ImageProcessor::ImageProcessor(QObject *parent) :
    QObject(parent),
    whiteGain_r(1),
    whiteGain_g(1),
    whiteGain_b(1)
{
}

ImageProcessor::~ImageProcessor()
{
}

void ImageProcessor::calibrateWhite(QString imagePath)
{
    Mat rawImage;
    rawImage = imread(imagePath.toStdString(), CV_LOAD_IMAGE_COLOR);   // Read the file

    size_t gridRow = rawImage.rows / 21;
    size_t gridCol = rawImage.cols / 21;
    size_t roiWidth = gridRow * 1;
    size_t roiHeight = gridCol * 1;
    qDebug() << "gridRow: " << gridRow << " " << "gridCol: " << gridCol;
    Mat image = rawImage(Rect((rawImage.cols - roiWidth) / 2,
                              (rawImage.rows - roiHeight) / 2,
                              roiWidth, roiHeight));

    // Noise reduction
    GaussianBlur(image, image, Size(3, 3),  0, 0);

    size_t rows = image.rows;
    size_t cols = image.cols;

    const uchar whiteThreshold = 32;
    average_r = 255;
    average_g = 255;
    average_b = 255;

    size_t effectivePixelCount = 0;
    const float effectivePercentage = 0.05f;
    effectivePixelCount = rows * cols * effectivePercentage;

    calcAverageForTopPercentageBrightestColour(image, rows, cols, effectivePixelCount, whiteThreshold,
            average_r, average_g, average_b);

    whiteGain_b = 255.0f / average_b;
    whiteGain_g = 255.0f / average_g;
    whiteGain_r = 255.0f / average_r;

    qDebug() << "average white (RGB): " << average_r << " " << average_g << " " << average_b;
    qDebug() << "white gain (RGB): " << whiteGain_r << " " << whiteGain_g << " " << whiteGain_b;

    processingResult =  "white: " +
                        formatRgb(average_r, average_g, average_b) +
                        formatRgb(whiteGain_r, whiteGain_g, whiteGain_b);

    //emit finishedCalibrateWhite();
}

void ImageProcessor::detectColour(QString imagePath)
{
    Mat rawImage;
    rawImage = imread(imagePath.toStdString(), CV_LOAD_IMAGE_COLOR);   // Read the file

    size_t gridRow = rawImage.rows / 21;
    size_t gridCol = rawImage.cols / 21;
    size_t roiWidth = gridRow * 1;
    size_t roiHeight = gridCol * 1;
    //qDebug() << "gridRow: " << gridRow << " " << "gridCol: " << gridCol;
    Mat image = rawImage(Rect((rawImage.cols - roiWidth) / 2,
                              (rawImage.rows - roiHeight) / 2,
                              roiWidth, roiHeight));

    // Noise reduction
    GaussianBlur(image, image, Size(3, 3),  0, 0);

    size_t rows = image.rows;
    size_t cols = image.cols;

    average_b = 0;
    average_g = 0;
    average_r = 0;

    size_t effectivePixelCount = 0;
    const float effectivePercentage = 1.0f;
    effectivePixelCount = rows * cols;

    // apply the 'white gain' for each channel
    for(size_t r = 0; r < rows; ++r) {
        for(size_t c = 0; c < cols; ++c) {
            RGB& rgb = image.ptr<RGB>(r)[c];

            average_r += rgb.red;
            average_g += rgb.green;
            average_b += rgb.blue;

            capValueTo255(rgb.blue * whiteGain_b, rgb.blue);
            capValueTo255(rgb.green * whiteGain_g, rgb.green);
            capValueTo255(rgb.red * whiteGain_r, rgb.red);

        }
    }

    average_r = average_r / effectivePixelCount;
    average_g = average_g / effectivePixelCount;
    average_b = average_b / effectivePixelCount;

    QString rawRgbString = formatRgb(average_r, average_g, average_b);

    effectivePixelCount = rows * cols * effectivePercentage;

    calcAverageForTopPercentageBrightestColour(image, rows, cols, effectivePixelCount, 0,
            average_r, average_g, average_b);

    qDebug() << "colour effectivePixelCount: " << effectivePixelCount;

    processingResult = rawRgbString + " -> " + formatRgb(average_r, average_g, average_b);

    qDebug() << "average colour (RGB): " << processingResult;

    //emit finishedDetectColour();
}

void ImageProcessor::getAnalysedRgbValue(RGB & rgb) const
{
    rgb.red = average_r;
    rgb.green = average_g;
    rgb.blue = average_b;
}
