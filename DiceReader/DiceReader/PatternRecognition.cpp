#include "stdafx.h"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

template <typename PixelType>
float ScorePatternInImageBlock(
    const cv::Mat& image,
    const PatternMask<PixelType>& patternMask,
    const cv::Rect& imageBlock)
{
    const cv::Mat cropped = image(imageBlock);
    assert(patternMask.BinaryPatternMask.size == cropped.size);

    // All pixels included by the pattern mask should have the same value.
    // All pixels excluded by the pattern mask should have the same value.
    std::vector<float> includedPixelScores;
    std::vector<float> maskedPixelScores;
    includedPixelScores.reserve(cropped.rows * cropped.cols);
    maskedPixelScores.reserve(cropped.rows * cropped.cols);

    for (int iRow = 0; iRow < cropped.rows; ++iRow)
    {
        for (int iCol = 0; iCol < cropped.cols; ++iCol)
        {
            const PixelType croppedVal = cropped.at<PixelType>(iRow, iCol);
            if (patternMask.BinaryPatternMask.at<uint8_t>(iRow, iCol) == 0)
            {
                maskedPixelScores.emplace_back(static_cast<float>(croppedVal - patternMask.MaskedColor));
            }
            else
            {
                includedPixelScores.emplace_back(static_cast<float>(croppedVal - patternMask.IncludedColor));
            }
        }
    }

    // Do something to get a final score here.
    // For example, check that the mean of both includedPixelScores and maskedPixelScores are close to 0,
    // and that the variances are also close to 0.

    return 128.f;
}

/////////////////////////////////////////////////
template <typename PixelType>
cv::Mat FindPatternInImage(
    const cv::Mat& image,
    const PatternMask<PixelType>& patternMask)
{
    const int startRow = 0;
    const int endRow = image.rows - patternMask.BinaryPatternMask.rows;
    const int startCol = 0;
    const int endCol = image.cols - patternMask.BinaryPatternMask.cols;

    cv::Mat scoreImage(image.rows, image.cols, cv::DataType<uint8_t>::type);
    scoreImage = cv::Scalar(0);

    for (int iRow = startRow; iRow < endRow; ++iRow)
    {
        for (int iCol = startCol; iCol < endCol; ++iCol)
        {
            const cv::Rect imageBlock(iCol, iRow, patternMask.BinaryPatternMask.cols, patternMask.BinaryPatternMask.rows);
            const float currentScore = ScorePatternInImageBlock(
                image,
                patternMask,
                imageBlock);

            scoreImage.at<uint8_t>(iRow, iCol) = static_cast<uint8_t>(currentScore);
        }
    }

    return scoreImage;

    // Do something with all the scores.
    // For example, accumulate all the scores and vote for the best image block.
    // Another example, just keep the best score and image block.
}

/////////////////////////////////////////////////
// Template declarations
template cv::Mat FindPatternInImage<uint8_t>(
    const cv::Mat& image,
    const PatternMask<uint8_t>& patternMask);