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

int main(int argc, char** argv)
{
    ApplyPerFrameManipulationToWebcamStream();

    return 0;
}