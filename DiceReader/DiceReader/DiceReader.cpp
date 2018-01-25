// DiceReader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace cv;
using namespace std;

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

    const int kernelRadius = 20;
    Mat imageBlurred = CreateBlurredImage(image, kernelRadius, ImageBlurType::Bilateral);

    // Name windows.
    String windowNameOriginal = "Original image";
    String windowNameChanged  = "Blurred image";

    // Create windows.
    namedWindow(windowNameOriginal);
    namedWindow(windowNameChanged);

    // Show images.
    imshow(windowNameOriginal, image);
    imshow(windowNameChanged, imageBlurred);

    waitKey(0); // Wait for any keystroke in the window
    destroyAllWindows();

    return 0;
}