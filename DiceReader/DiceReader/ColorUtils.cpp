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
void DisplayColorSpace(const ColorSpace colorSpace)
{
    // Create new image.
    const int numRows = 600;
    const int numCols = 800;
    cv::Mat image(numRows, numCols, CV_8UC3, cv::Scalar(0, 0, 0));

    // Create window controls.
    std::string controlWindowName = "Control";
    cv::namedWindow(controlWindowName, CV_WINDOW_AUTOSIZE); //create a window called "Control"

    //Create trackbars in control window
    int low[3] = { 0, 0, 0 };
    int high[3] = { 255, 255, 255 };

    TrackbarCallbackData data0(0, image);
    TrackbarCallbackData data1(1, image);
    TrackbarCallbackData data2(2, image);

    std::string colorChannelNames[3];
    cv::String windowName;
    switch (colorSpace)
    {
    case ColorSpace::BGR:
        colorChannelNames[0] = "Blue";
        colorChannelNames[1] = "Green";
        colorChannelNames[2] = "Red";
        windowName = "BGR";
        break;
    case ColorSpace::HSV:
        colorChannelNames[0] = "Hue";
        colorChannelNames[1] = "Saturation";
        colorChannelNames[2] = "Value";
        windowName = "HSV";
        break;
    case ColorSpace::YUV:
        colorChannelNames[0] = "Luma";
        colorChannelNames[1] = "ChromaB";
        colorChannelNames[2] = "ChromaR";
        windowName = "YUV";
        break;
    default:
        std::cerr << "Unknown color space." << std::endl;
        exit(1);
    }

    cv::createTrackbar(colorChannelNames[0], controlWindowName.c_str(), &low[0], high[0], CallbackTrackbar, &data0);
    cv::createTrackbar(colorChannelNames[1], controlWindowName.c_str(), &low[1], high[1], CallbackTrackbar, &data1);
    cv::createTrackbar(colorChannelNames[2], controlWindowName.c_str(), &low[2], high[2], CallbackTrackbar, &data2);

    // Name windows.
    namedWindow(windowName);

    // Show images.
    cv::Mat bgrImage;
    while (true)
    {
        switch (colorSpace)
        {
        case ColorSpace::BGR:
            bgrImage = image;
            break;
        case ColorSpace::HSV:
            cv::cvtColor(image, bgrImage, CV_HSV2BGR);
            break;
        case ColorSpace::YUV:
            cv::cvtColor(image, bgrImage, CV_YUV2BGR);
            break;
        default:
            std::cerr << "Unknown color space." << std::endl;
            exit(1);
        }
        imshow(windowName, bgrImage);

        if (cv::waitKey(30) != -1) // Wait 30 msec for any keystroke in the window
        {
            break; 
        }
    }
    cv::destroyAllWindows(); 
}
