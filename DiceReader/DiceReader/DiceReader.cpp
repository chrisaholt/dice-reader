// DiceReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>

using namespace cv;
using namespace std;

Mat ChangeBrightnessOfImage(const Mat& image, const bool shouldBrighten)
{
    Mat newImage = image.clone();

    //const Scalar valToAdd = shouldBrighten ? Scalar(75, 75, 75) : Scalar(-75, -75, -75);
    //newImage = image + valToAdd;
    //newImage = 2.0f * image;

    const int colorThresh = 150;
    const float colorScale = shouldBrighten ? 3.0f : 1/3.0f;
    for (int y = 0; y < newImage.rows; ++y)
    {
        for (int x = 0; x < newImage.cols; ++x)
        {
            Vec3b color = newImage.at<Vec3b>(Point(x, y));
            for (int iColor = 0; iColor < 3; ++iColor)
            {
                if (shouldBrighten && color[iColor] > colorThresh)
                {
                    color[iColor] = saturate_cast<uchar>((color[iColor] - colorThresh) * colorScale + colorThresh);
                }
                else if (!shouldBrighten && color[iColor] < colorThresh)
                {
                    color[iColor] = saturate_cast<uchar>((color[iColor] - colorThresh) * colorScale + colorThresh);
                }
            }
            newImage.at<Vec3b>(Point(x, y)) = color;
        }
    }

    return newImage;
}

int main(int argc, char** argv)
{
    // Read the image file
    Mat image = imread("Images/paint_lowLowRes.jpg", CV_LOAD_IMAGE_COLOR);

    if (image.empty()) // Check for failure
    {
        cout << "Could not open or find the image" << endl;
        system("pause"); //wait for any key press
        return -1;
    }

    Mat imageBrighter = ChangeBrightnessOfImage(image, true);
    Mat imageDarker   = ChangeBrightnessOfImage(image, false);

    // Name windows.
    String windowNameOriginal = "Original image";
    String windowNameBrighter = "Brighter image";
    String windowNameDarker   = "Darker image";

    // Create windows.
    namedWindow(windowNameOriginal);
    namedWindow(windowNameBrighter);
    namedWindow(windowNameDarker);

    // Show images.
    imshow(windowNameOriginal, image);
    imshow(windowNameBrighter, imageBrighter);
    imshow(windowNameDarker, imageDarker);

    waitKey(0); // Wait for any keystroke in the window
    destroyAllWindows();

    return 0;
}