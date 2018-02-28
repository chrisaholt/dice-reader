#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace cv;

/////////////////////////////////////////////////
Mat CreateBlurredImage(
    const Mat& image, 
    const int kernelRadius,
    const ImageBlurType blurType)
{
    const int kernelDiameter = 2*kernelRadius + 1;  // Required to be odd.
    const Size kernelSize(kernelDiameter, kernelDiameter);

    Mat blurredImage;
    switch (blurType)
    {
    case ImageBlurType::NormalizedBox:
        blur(image, blurredImage, kernelSize);
        break;
    case ImageBlurType::Gaussian:
        GaussianBlur(image, blurredImage, kernelSize, 0, 0);
        break;
    case ImageBlurType::Median:
        medianBlur(image, blurredImage, kernelDiameter);
        break;
    case ImageBlurType::Bilateral:
        bilateralFilter(image, blurredImage, kernelDiameter, kernelDiameter, kernelDiameter);
        break;
    default:
        throw(std::invalid_argument("Unknown blurType in CreateBlurredImage()."));
        break;
    }

    return blurredImage;
}

/////////////////////////////////////////////////
cv::Mat CreateEdgeImage(
    const cv::Mat& image)
{
    // Blur image to reduce noise.
    const int kernelRadius = 5;    
    const Mat blurredImage = CreateBlurredImage(image, kernelRadius, ImageBlurType::Median);
    //const Mat blurredImage = CreateBlurredImage(image, kernelRadius, ImageBlurType::Bilateral);

    // Alternative 1.
    //int depth = -1; 
    //Mat imageLaplacian;
    //cv::Laplacian(blurredImage, imageLaplacian, depth);
    //return imageLaplacian;

    // Alternative 2.
    const double thresh1 = 50;
    const double thresh2 = 3 * thresh1;
    const int cannyKernelSize = 3;
    const bool shouldUseL2Gradient = true;
    cv::Mat edgeImage;
    cv::Canny(blurredImage, edgeImage, thresh1, thresh2, cannyKernelSize, shouldUseL2Gradient);
    return edgeImage;
}

/////////////////////////////////////////////////
Mat CreateHistogramOfImage(
    const Mat& image)
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

/////////////////////////////////////////////////
cv::Mat DrawCirclesOnImage(
    const cv::Mat& image)
{
    cv::Mat imageWithDrawnCircles = image.clone();

    cv::Mat grayscaleImage;
    cvtColor(image, grayscaleImage, CV_BGR2GRAY);
    std::vector<cv::Vec3f> circles;
    const double minDistBetweenCircleCenters = 25;
    const int minRadius = 0;
    const int maxRadius = 30;
    cv::HoughCircles(
        grayscaleImage,
        circles, 
        CV_HOUGH_GRADIENT,
        1, 
        minDistBetweenCircleCenters, 
        100, 
        15, 
        minRadius, 
        maxRadius);

    const cv::Scalar circleColor(0, 255, 0);
    const int circleLineWidth = 1;

    // Loop over all detected circles and outline them on the original image
    for (size_t iCircle = 0; iCircle < circles.size(); ++iCircle)
    {
        cv::Point center(
            static_cast<int>(std::round(circles[iCircle][0])),
            static_cast<int>(std::round(circles[iCircle][1])));
        const int radius =
            static_cast<int>(std::round(circles[iCircle][2]));
        cv::circle(imageWithDrawnCircles, center, radius, circleColor, circleLineWidth);
    }

    return imageWithDrawnCircles;
}

/////////////////////////////////////////////////
template <typename PixelType>
cv::Mat MaskImage(
    const cv::Mat& image,
    std::function<bool(const PixelType&)> PixelMask,
    const PixelType& maskedValue)
{
    Mat masked;
    const bool shouldUseHSVColorSpace = true;
    if (shouldUseHSVColorSpace)
    {
        cv::cvtColor(image, masked, CV_BGR2HSV);
    }
    else
    {
        masked = image.clone();
    }

    masked.forEach<PixelType>([&PixelMask, &maskedValue](PixelType &pixel, const int * position) {
        if (!PixelMask(pixel))
        {
            pixel = maskedValue;
        }
    });

    if (shouldUseHSVColorSpace)
    {
        cv::cvtColor(masked, masked, CV_HSV2BGR);
    }
    return masked;
}

/////////////////////////////////////////////////
void ReadAndDisplayImage(const std::string& imageFilename)
{
    // Read the image file
    const cv::Mat image = imread(imageFilename, CV_LOAD_IMAGE_COLOR);

    if (image.empty()) // Check for failure
    {
        std::cout << "Could not open or find the image." << std::endl;
        exit(1);
    }

    // Name windows.
    String windowName = "Image";

    // Create windows.
    namedWindow(windowName);

    // Show images.
    imshow(windowName, image);

    waitKey(0); // Wait for any keystroke in the window
    destroyAllWindows();
}

/////////////////////////////////////////////////
void ReadAndDisplayImageAndManipulation(
    const std::string& imageFilename,
    std::function<cv::Mat(const cv::Mat&)> ImageManipulation)
{
    // Read the image file
    const cv::Mat image = imread(imageFilename, CV_LOAD_IMAGE_COLOR);

    if (image.empty()) // Check for failure
    {
        std::cout << "Could not open or find the image." << std::endl;
        exit(1);
    }

    const cv::Mat manipulatedImage = ImageManipulation(image);

    // Name windows.
    String windowImageName = "Image";
    String windowManipulationName = "Manipulation";

    // Create windows.
    namedWindow(windowImageName);
    namedWindow(windowManipulationName);

    // Show images.
    imshow(windowImageName, image);
    imshow(windowManipulationName, manipulatedImage);

    waitKey(0); // Wait for any keystroke in the window
    destroyAllWindows();
}

/////////////////////////////////////////////////
cv::Mat SharpenEdges(
    const cv::Mat& image)
{
    const Mat edgeImage = CreateEdgeImage(image);
    const Mat smoothedEdgeImage = CreateBlurredImage(
        edgeImage,
        3, // kernelRadius
        ImageBlurType::Median);

    const int scale = 10;   // TODO: Make choice of scale more dynamic.
    const Mat sharpenedImage = image - scale * smoothedEdgeImage;
    return sharpenedImage;
}

/////////////////////////////////////////////////
// Template declarations
template cv::Mat MaskImage<cv::Point3_<uint8_t>>(
    const cv::Mat& image,
    std::function<bool(const cv::Point3_<uint8_t>&)> PixelMask,
    const cv::Point3_<uint8_t>& maskedValue);
