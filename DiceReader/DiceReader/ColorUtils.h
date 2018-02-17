#pragma once

#include <opencv2/opencv.hpp>

enum class ColorSpace
{
    Unknown,
    BGR,
    HSV
};

void DisplayColorSpace(const ColorSpace colorSpace);
