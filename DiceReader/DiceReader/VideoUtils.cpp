#include "stdafx.h"

void ApplyPerFrameManipulationToWebcamStream(
    // Function ImageManipulationTechnique
)
{
    cv::VideoCapture webcam(0);
    if (!webcam.isOpened())
    {
        std::cout << "Cannot open webcam." << std::endl;
        exit(1);
    }

    // Create window controls.
    std::string controlWindowName = "Control";
    cv::namedWindow(controlWindowName, CV_WINDOW_AUTOSIZE); //create a window called "Control"

    int kernelRadiusLow = 1;
    int kernelRadiusHigh = 20;

    //Create trackbars in control window
    cvCreateTrackbar("Kernel Radius", controlWindowName.c_str(), &kernelRadiusLow, kernelRadiusHigh);

    while (true)
    {
        // Read image from webcam.
        cv::Mat imgOriginal;
        bool couldReadFrame = webcam.read(imgOriginal);

        if (!couldReadFrame)
        {
            std::cout << "Cannot read a frame from video stream." << std::endl;
            break;
        }

        // Blur image.
        const int kernelRadius = kernelRadiusLow;
        const cv::Mat blurredImage = CreateBlurredImage(
            imgOriginal,
            kernelRadius,
            ImageBlurType::Bilateral);

        // Show images.
        cv::imshow("Original", imgOriginal);
        cv::imshow("Updated", blurredImage);

        if (cv::waitKey(30) == 27) // Wait for ESC key press for 30ms.
        {
            std::cout << "ESC key is pressed by user." << std::endl;
            break;
        }
    }
}
