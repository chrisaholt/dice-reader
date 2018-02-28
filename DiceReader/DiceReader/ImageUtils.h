#pragma once
#include <opencv2/opencv.hpp>

enum class ImageBlurType {
    Unknown = 0,
    NormalizedBox,
    Gaussian,
    Median,
    Bilateral
};


cv::Mat CreateBlurredImage(
    const cv::Mat& image,
    const int kernelRadius,
    const ImageBlurType blurType);

cv::Mat CreateEdgeImage(
    const cv::Mat& image);

cv::Mat CreateHistogramOfImage(
    const cv::Mat& image);

cv::Mat DrawCirclesOnImage(
    const cv::Mat& image);

// Example PixelMask is intensity-based thresholding (less than or greater than).
template <typename PixelType>
cv::Mat MaskImage(
    const cv::Mat& image,
    std::function<bool(const PixelType&)> PixelMask,
    const PixelType& maskedValue);

void ReadAndDisplayImage(
    const std::string& imageFilename);

void ReadAndDisplayImageAndManipulation(
    const std::string& imageFilename,
    std::function<cv::Mat(const cv::Mat&)> ImageManipulation);

cv::Mat SharpenEdges(
    const cv::Mat& image);