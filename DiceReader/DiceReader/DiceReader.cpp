// DiceReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace cv;
using namespace std;

void ReadAndDisplayImage(const std::string& imageFilename)
{
    // Read the image file
    Mat image = imread(imageFilename, CV_LOAD_IMAGE_COLOR);
    //Mat image = imread("Images/paint_lowLowRes.jpg", CV_LOAD_IMAGE_COLOR);

    if (image.empty()) // Check for failure
    {
        cout << "Could not open or find the image." << endl;
        system("pause"); //wait for any key press
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

bool DetectRedInHsv(const cv::Point3_<uint8_t>& pixel)
{
    const uint8_t hueThresh1[2] = { 0, 15 }; // Red is between these values.
    const uint8_t hueThresh2[2] = { 160, 179 }; // Red is between these values.
    const uint8_t saturationThresh[2] = {100, 255}; // 100, 255
    const uint8_t valueThresh[2] = {100, 255};  // 150, 255

    if (hueThresh1[0] <= pixel.x && pixel.x <= hueThresh1[1] &&
        saturationThresh[0] <= pixel.y && pixel.y <= saturationThresh[1] &&
        valueThresh[0] <= pixel.z && pixel.z <= valueThresh[1])
    {
        return true;
    }

    if (hueThresh2[0] <= pixel.x && pixel.x <= hueThresh2[1] &&
        saturationThresh[0] <= pixel.y && pixel.y <= saturationThresh[1] &&
        valueThresh[0] <= pixel.z && pixel.z <= valueThresh[1])
    {
        return true;
    }

    return false;
}

int main(int argc, char** argv)
{
    const cv::Point3_<uint8_t> maskedValue(0, 0, 0);
    ApplyPerFrameManipulationToWebcamStream(
        [&maskedValue](const cv::Mat& image) {
        const cv::Mat blurredImage = CreateBlurredImage(
            image,
            1, // kernelRadius
            ImageBlurType::Median);
        return MaskImage<cv::Point3_<uint8_t>>(
            blurredImage,
            DetectRedInHsv,
            maskedValue);
    }
    );

    return 0;
}