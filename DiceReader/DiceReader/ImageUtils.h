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

cv::Mat SharpenEdges(
    const cv::Mat& image);