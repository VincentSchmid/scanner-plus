#include "scanner/imageProcessing.h"
#include <opencv2/opencv.hpp>


std::vector<uchar> get_jpeg_buffer(cv::Mat img)
{
    std::vector<uchar> buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(90);

    cv::imencode(".jpg", img, buffer, params);

    return buffer;
}

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

cv::Mat adjustBrightnessContrast(const cv::Mat& input, double brightness, double contrast)
{
    cv::Mat output;
    double alpha = 1 + contrast / 100.0;
    double beta = brightness;

    input.convertTo(output, -1, alpha, beta);
    return output;
}

const std::vector<cv::Point2f> getDocumentContours(cv::Mat img)
{
    cv::Mat smallImage;

    // Resize the smallImage
    cv::resize(img, smallImage, cv::Size(), 0.5, 0.5);

    // Create a mask based on the known background color
    cv::Mat mask;
    cv::Mat gray;
    cv::cvtColor(smallImage, gray, cv::COLOR_RGB2GRAY);
    cv::adaptiveThreshold(gray, mask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

     // Erode the entire mask
    cv::Mat erodedMask;
    cv::erode(mask, mask, kernel, cv::Point(-1, -1), 1);
    cv::dilate(mask, mask, kernel, cv::Point(-1, -1), 1);

    // 2. Find Contours
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Find the contour with the most points
    std::vector<cv::Point> docContour;
    if (!contours.empty()) {
        std::sort(contours.begin(), contours.end(), [](const std::vector<cv::Point>& c1, const std::vector<cv::Point>& c2) {
            return cv::contourArea(c1, false) > cv::contourArea(c2, false);
        });

        docContour = contours[0];
    }

    std::vector<cv::Point> displayContour;

    double peri = cv::arcLength(docContour, true);
    std::vector<cv::Point> approx;

    double epsilon = 0.01 * peri;
    const double maxEpsilon = 0.3 * peri;
    const double increment = 0.01 * peri;

     while (epsilon <= maxEpsilon) {
        cv::approxPolyDP(docContour, approx, epsilon, true);
        if (approx.size() == 4) {
            displayContour = approx;
            break;
        }
        epsilon += increment;
    }

    cv::Point2f bottomRight;

    for (cv::Point& pt : displayContour) {
        if (pt.x > bottomRight.x) {
            bottomRight.x = pt.x;
        }
        if (pt.y > bottomRight.y) {
            bottomRight.y = pt.y;
        }
    }

    std::vector<cv::Point2f> result = {
        cv::Point2f(0, 0),
        cv::Point2f(bottomRight.x, 0),
        cv::Point2f(bottomRight.x, bottomRight.y),
        cv::Point2f(0, bottomRight.y)
    };

    for (cv::Point2f& pt : result) {
        pt.x *= 2;
        pt.y *= 2;
    }

    return result;
}

const cv::Mat crop_image(cv::Mat img, std::vector<cv::Point2f> cropPoints)
{
    //return fourPointTransform(img, cropPoints);
    //simple crop for now
    cv::Rect rect(cropPoints[0], cropPoints[2]);
    cv::Mat croppedImage = img(rect);
    return croppedImage;
}

std::vector<cv::Point2f> order_points(const std::vector<cv::Point>& pts) {
    std::vector<cv::Point2f> rect(4);
    if (pts.size() != 4) {
        return rect; // Return empty if not 4 points
    }

    // Compute the centroid of the contour
    cv::Point2f centroid(0, 0);
    for (const auto& pt : pts) {
        centroid.x += pt.x;
        centroid.y += pt.y;
    }
    centroid.x /= 4;
    centroid.y /= 4;

    // Classify each point based on its position relative to the centroid
    for (const auto& pt : pts) {
        if (pt.x < centroid.x) {
            if (pt.y < centroid.y) {
                rect[0] = pt; // top-left
            } else {
                rect[3] = pt; // bottom-left
            }
        } else {
            if (pt.y < centroid.y) {
                rect[1] = pt; // top-right
            } else {
                rect[2] = pt; // bottom-right
            }
        }
    }

    return rect;
}

cv::Mat fourPointTransform(const cv::Mat& image, const std::vector<cv::Point2f>& rect) {
    float widthA = std::sqrt(std::pow(rect[2].x - rect[3].x, 2) + std::pow(rect[2].y - rect[3].y, 2));
    float widthB = std::sqrt(std::pow(rect[1].x - rect[0].x, 2) + std::pow(rect[1].y - rect[0].y, 2));
    int maxWidth = std::max(static_cast<int>(widthA), static_cast<int>(widthB));

    float heightA = std::sqrt(std::pow(rect[1].x - rect[2].x, 2) + std::pow(rect[1].y - rect[2].y, 2));
    float heightB = std::sqrt(std::pow(rect[0].x - rect[3].x, 2) + std::pow(rect[0].y - rect[3].y, 2));
    int maxHeight = std::max(static_cast<int>(heightA), static_cast<int>(heightB));

    std::vector<cv::Point2f> dst = {
        {0, 0},
        {static_cast<float>(maxWidth - 1), 0},
        {static_cast<float>(maxWidth - 1), static_cast<float>(maxHeight - 1)},
        {0, static_cast<float>(maxHeight - 1)}
    };

    std::vector<cv::Point2f> rectVec(rect.begin(), rect.end());
    cv::Mat M = cv::getPerspectiveTransform(rectVec, dst);
    cv::Mat warped;
    cv::warpPerspective(image, warped, M, cv::Size(maxWidth, maxHeight));

    return warped;
}
