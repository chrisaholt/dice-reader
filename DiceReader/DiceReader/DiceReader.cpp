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

bool ThresholdPixel(const cv::Point3_<uint8_t>& pixel)
{
    const uint8_t blueThresh = 150;
    const uint8_t greenThresh = 150;
    const uint8_t redThresh = 150;
    if (pixel.x >= blueThresh &&
        pixel.y >= greenThresh &&
        pixel.z >= redThresh)
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
        return MaskImage<cv::Point3_<uint8_t>>(
            image,
            ThresholdPixel,
            maskedValue);
    }
    );

    return 0;
}