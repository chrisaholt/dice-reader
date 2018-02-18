#pragma once

#include <opencv2/opencv.hpp>

enum class ColorSpace
{
    Unknown,
    BGR,
    HSV,
    YUV
};

bool DetectBlueInHsv(const cv::Point3_<uint8_t>& pixel);

bool DetectRedInHsv(const cv::Point3_<uint8_t>& pixel);

bool DetectWhiteInHsv(const cv::Point3_<uint8_t>& pixel);

void DisplayColorSpace(const ColorSpace colorSpace);
