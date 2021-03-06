// DiceReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace cv;
using namespace std;

std::function<cv::Mat(const cv::Mat&)> CreateImageManipulation()
{
    std::function<cv::Mat(const cv::Mat&)> imageManipulation;

    const bool shouldBlurImage = false;
    if (shouldBlurImage)
    {
        // Create window controls.
        std::string controlWindowName = "Control";
        cv::namedWindow(controlWindowName, CV_WINDOW_AUTOSIZE); //create a window called "Control"

        int kernelRadiusLow = 1;
        int kernelRadiusHigh = 20;

        //Create trackbars in control window
        cvCreateTrackbar("Kernel Radius", controlWindowName.c_str(), &kernelRadiusLow, kernelRadiusHigh);

        //Blur image.
        imageManipulation = [&kernelRadiusLow](const cv::Mat& image)
        { return CreateBlurredImage(
            image,
            kernelRadiusLow,
            ImageBlurType::Bilateral); };
    }
    else // shouldUseEdgeImage
    {
        imageManipulation =
            [](const cv::Mat& image) { return 3 * abs(CreateEdgeImage(image)); };
    }

    return imageManipulation;
}

int main(int argc, char** argv)
{
    // Want to detect squares (dice-shaped regions), 
    // then detect circles within the squares.
    // Read up at https://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html
    // Also look at MSER at https://docs.opencv.org/2.4/modules/features2d/doc/feature_detection_and_description.html

    //const cv::Point3_<uint8_t> maskedValue(0, 0, 0);
    //ApplyPerFrameManipulationToWebcamStream(
    //    [&maskedValue](const cv::Mat& image) {
    //    const cv::Mat blurredImage = CreateBlurredImage(
    //        image,
    //        1, // kernelRadius
    //        ImageBlurType::Median);
    //    const cv::Mat maskedImage = MaskImage<cv::Point3_<uint8_t>>(
    //        blurredImage,
    //        DetectBlueInHsv,
    //        maskedValue);
    //    return DrawCirclesOnImage(maskedImage);
    //}
    //);

    auto ScoreImage = [](const cv::Mat&image)
    {
        cv::Mat grayscaleImage;
        cvtColor(image, grayscaleImage, CV_BGR2GRAY);
        PatternMask<uint8_t> patternMask;
        patternMask.BinaryPatternMask = std::move(cv::Mat(20, 20, cv::DataType<uint8_t>::type));
        patternMask.IncludedColor = static_cast<uint8_t>(255);
        patternMask.MaskedColor = static_cast<uint8_t>(0);

        return FindPatternInImage(
            grayscaleImage,
            patternMask);
    };

    std::vector<std::string> diceImages;
    diceImages.push_back("Images/die_blue_2_lowRes.jpg");
    diceImages.push_back("Images/die_blue_3_lowRes.jpg");
    diceImages.push_back("Images/die_blue_4_lowRes.jpg");
    diceImages.push_back("Images/die_blue_6_lowRes.jpg");
    diceImages.push_back("Images/die_blue_4_lowLowRes.jpg");

    std::string imageFilename = diceImages[4];
    ReadAndDisplayImageAndManipulation(
        imageFilename,  
        ScoreImage);

    return 0;
}