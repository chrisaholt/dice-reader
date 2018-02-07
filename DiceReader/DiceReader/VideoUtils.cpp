#include "stdafx.h"

void ApplyPerFrameManipulationToWebcamStream(
    std::function<cv::Mat(const cv::Mat&)> ImageManipulation
)
{
    const int cameraIndex = 1; // 0 = front camera; 1 = back camera
    cv::VideoCapture webcam(cameraIndex);
    if (!webcam.isOpened())
    {
        std::cout << "Cannot open webcam." << std::endl;
        exit(1);
    }

    while (true)
    {
        // Read image from webcam.
        cv::Mat imgOriginal;
        const bool couldReadFrame = webcam.read(imgOriginal);

        if (!couldReadFrame)
        {
            std::cout << "Cannot read a frame from video stream." << std::endl;
            break;
        }

        const bool shouldUseGrayscale = true;
        if (shouldUseGrayscale)
        {
            cv::cvtColor(imgOriginal, imgOriginal, CV_BGR2GRAY);
        }

        const cv::Mat updatedImage = ImageManipulation(imgOriginal);

        // Show images.
        cv::imshow("Original", imgOriginal);
        cv::imshow("Updated", updatedImage);

        if (cv::waitKey(30) == 27) // Wait for ESC key press for 30ms.
        {
            std::cout << "ESC key is pressed by user." << std::endl;
            break;
        }
    }
}
