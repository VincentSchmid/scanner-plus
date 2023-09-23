#include <opencv2/opencv.hpp>


const cv::Mat sharpen_image(cv::Mat img)
{
    // Blur the image using Gaussian blur
    cv::Mat imgBlurred;
    cv::GaussianBlur(img, imgBlurred, cv::Size(0, 0), 3);

    // Subtract the blurred image from the original (scaled) to get the "mask"
    cv::Mat mask = img - imgBlurred;

    // Then add the "mask" back to the original image, scaling it to control sharpness
    float sharpness_factor = 0.5;
    cv::Mat imgSharp = img + sharpness_factor * mask;

    // Convert back to 8-bit before saving
    imgSharp.convertTo(imgSharp, CV_8UC3);

    return imgSharp;
}

const cv::Mat auto_crop_image(cv::Mat img)
{
    // Convert to grayscale
    cv::Mat imgGray;
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);

    // Threshold the image
    cv::Mat imgThresh;
    cv::threshold(imgGray, imgThresh, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // Find the contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(imgThresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Find the largest contour
    int largest_area = 0;
    int largest_contour_index = 0;
    for (int i = 0; i < contours.size(); i++)
    {
        double area = cv::contourArea(contours[i]);
        if (area > largest_area)
        {
            largest_area = area;
            largest_contour_index = i;
        }
    }

    // Find the bounding rectangle of the largest contour
    cv::Rect bounding_rect = cv::boundingRect(contours[largest_contour_index]);

    // Crop the image to the bounding rectangle
    cv::Mat imgCropped = img(bounding_rect);

    return imgCropped;
}
