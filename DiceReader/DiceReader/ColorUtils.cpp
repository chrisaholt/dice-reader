#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


/////////////////////////////////////////////////
struct TrackbarCallbackData
{
    int ColorIndex;
    cv::Mat* ImagePtr;

    TrackbarCallbackData(const int colorIndex, cv::Mat& image) 
        : ColorIndex(colorIndex)
        , ImagePtr(&image)
    {
    }
};

/////////////////////////////////////////////////
static void CallbackTrackbar(int pos, void* data)
{
    TrackbarCallbackData* typedData = reinterpret_cast<TrackbarCallbackData*>(data);
    
    cv::Vec3b currentPixelValues = typedData->ImagePtr->at<cv::Vec3b>(0,0);
    currentPixelValues[typedData->ColorIndex] = static_cast<uint8_t>(pos);
    *(typedData->ImagePtr) = currentPixelValues;
}

/////////////////////////////////////////////////
void ShowRGBColorSpace()
{
    // Create new image.
    const int numRows = 600;
    const int numCols = 800;
    cv::Mat image(numRows, numCols, CV_8UC3, cv::Scalar(0, 0, 0));

    // Create window controls.
    std::string controlWindowName = "Control";
    cv::namedWindow(controlWindowName, CV_WINDOW_AUTOSIZE); //create a window called "Control"

    //Create trackbars in control window
    int bLow = 0;
    int bHigh = 255;
    int gLow = 0;
    int gHigh = 255;
    int rLow = 0;
    int rHigh = 255;

    TrackbarCallbackData bData(0, image);
    TrackbarCallbackData gData(1, image);
    TrackbarCallbackData rData(2, image);

    cv::createTrackbar("Blue", controlWindowName.c_str(), &bLow, bHigh, CallbackTrackbar, &bData);
    cv::createTrackbar("Green", controlWindowName.c_str(), &gLow, gHigh, CallbackTrackbar, &gData);
    cv::createTrackbar("Red",   controlWindowName.c_str(), &rLow, rHigh, CallbackTrackbar, &rData);

    // Name windows.
    cv::String windowName = "Image";
    namedWindow(windowName);

    // Show images.
    while (true)
    {
        imshow(windowName, image);

        if (cv::waitKey(30) != -1) // Wait 30 msec for any keystroke in the window
        {
            break; 
        }
    }
    cv::destroyAllWindows(); 
}
