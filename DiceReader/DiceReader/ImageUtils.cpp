#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;

Mat CreateHistogramOfImage(const Mat& image)
{
    // Adapted from https://docs.opencv.org/2.4/doc/tutorials/imgproc/histograms/histogram_calculation/histogram_calculation.html.
    std::vector<Mat> colorChannels;
    split(image, colorChannels);
    const int blueIndex = 0;
    const int greenIndex = 1;
    const int redIndex = 2;

    // Histogram options.
    const int numBins = 256;
    float range[] = { 1, 255 };
    //float range[] = { 0, 256 };
    const float* histRange = { range };
    const bool shouldUseUniformBinSize = true;
    const bool shouldAccumulate = false;

    // Computer histograms.
    Mat blueHist, greenHist, redHist;
    calcHist(&colorChannels[blueIndex], 1, 0, Mat(), blueHist, 1, &numBins, &histRange, shouldUseUniformBinSize, shouldAccumulate);
    calcHist(&colorChannels[greenIndex], 1, 0, Mat(), greenHist, 1, &numBins, &histRange, shouldUseUniformBinSize, shouldAccumulate);
    calcHist(&colorChannels[redIndex], 1, 0, Mat(), redHist, 1, &numBins, &histRange, shouldUseUniformBinSize, shouldAccumulate);

    // Draw the histograms .
    const int histImageWidth = 512;
    const int histImageHeight = 400;
    const int binWidth = cvRound((double)histImageWidth / numBins);
    Mat histImage(histImageHeight, histImageWidth, CV_8UC3, Scalar(0, 0, 0));

    // Normalize the result to [ 0, histImage.rows ]
    normalize(blueHist, blueHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(greenHist, greenHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
    normalize(redHist, redHist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    // Draw for each channel
    for (int i = 1; i < numBins; i++)
    {
        const int binXStart = binWidth*(i - 1);
        const int binXEnd = binWidth*(i);
        const auto binYStart = [&histImageHeight, &i](const Mat& hist) { return histImageHeight - cvRound(hist.at<float>(i - 1)); };
        const auto binYEnd = [&histImageHeight, &i](const Mat& hist) { return histImageHeight - cvRound(hist.at<float>(i)); };
        line(histImage,
            Point(binXStart, binYStart(blueHist)),
            Point(binXEnd, binYEnd(blueHist)),
            Scalar(255, 0, 0), 2, 8, 0);
        line(histImage,
            Point(binXStart, binYStart(greenHist)),
            Point(binXEnd, binYEnd(greenHist)),
            Scalar(0, 255, 0), 2, 8, 0);
        line(histImage,
            Point(binXStart, binYStart(redHist)),
            Point(binXEnd, binYEnd(redHist)),
            Scalar(0, 0, 255), 2, 8, 0);
    }

    return histImage;
}