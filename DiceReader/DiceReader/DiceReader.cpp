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

void WatchWebcam()
{
    cv::VideoCapture webcam(0);
    if (!webcam.isOpened())
    {
        std::cout << "Cannot open webcam." << std::endl;
        exit(1);
    }

    // Create window controls.
    std::string controlWindowName = "Control";
    namedWindow(controlWindowName, CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int kernelRadiusLow = 1;
    int kernelRadiusHigh = 20;

    //Create trackbars in control window
    cvCreateTrackbar("Kernel Radius", controlWindowName.c_str(), &kernelRadiusLow, kernelRadiusHigh);

    while (true)
    {
        // Read image from webcam.
        Mat imgOriginal;
        bool couldReadFrame = webcam.read(imgOriginal);

        if (!couldReadFrame)
        {
            std::cout << "Cannot read a frame from video stream." << std::endl;
            break;
        }

        // Blur image.
        const int kernelRadius = kernelRadiusLow;
        const Mat blurredImage = CreateBlurredImage(
            imgOriginal,
            kernelRadius,
            ImageBlurType::Bilateral);

        // Show images.
        imshow("Original", imgOriginal);
        imshow("Updated", blurredImage);

        if (waitKey(30) == 27) // Wait for ESC key press for 30ms.
        {
            std::cout << "ESC key is pressed by user." << std::endl;
            break;
        }
    }

}

int main(int argc, char** argv)
{
    WatchWebcam();

    return 0;
}