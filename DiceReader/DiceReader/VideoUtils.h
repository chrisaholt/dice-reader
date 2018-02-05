#pragma once

#include <functional>

#include <opencv2/opencv.hpp>

void ApplyPerFrameManipulationToWebcamStream(
    std::function<cv::Mat(const cv::Mat&)> ImageManipulation
);
