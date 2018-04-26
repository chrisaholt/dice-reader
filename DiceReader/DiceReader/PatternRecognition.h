#pragma once

#include <opencv2/opencv.hpp>
#include <utility>

template <typename PixelType>
struct PatternMask
{
    PatternMask()
    {
    }

    cv::Mat BinaryPatternMask;
    PixelType IncludedColor;
    PixelType MaskedColor;
};

template <typename PixelType>
cv::Mat FindPatternInImage(
    const cv::Mat& image,
    const PatternMask<PixelType>& patternMask);
