#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>


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

cv::Mat fourPointTransform(const cv::Mat& image, const std::vector<cv::Point>& pts) {
    std::vector<cv::Point2f> rect = order_points(pts);

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

// Sample test case (modify as needed)
TEST(DocumentScannerTest, TestScanFunction) {
    // Read the image
    cv::Mat originalImage = cv::imread("test/test.png");

    // Check if image is loaded successfully
    if (originalImage.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return;
    }

    cv::Mat image = originalImage.clone();

    // Resize the image
    cv::resize(image, image, cv::Size(), 0.5, 0.5);

    // Blur the image to reduce color noise
    cv::Mat blurred;
    cv::GaussianBlur(image, blurred, cv::Size(5, 5), 0);

    // Create a mask based on the known background color
    cv::Mat mask;
    cv::inRange(blurred, cv::Scalar(228, 228, 228), cv::Scalar(240, 243, 243), mask);
    mask = 255 - mask;

    // Perform dilation to reduce noise
    cv::dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 3);

    // 1. Morphological Operations
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);

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

    // Draw the largest contour
    cv::Mat result;
    image.copyTo(result);
    cv::drawContours(result, std::vector<std::vector<cv::Point> >(1, docContour), -1, cv::Scalar(0, 255, 0), 2);

    std::vector<cv::Point> displayContour; // This will store the four-vertex contour

    double peri = cv::arcLength(docContour, true);
    std::vector<cv::Point> approx;

    double epsilon = 0.01 * peri;  // Starting value
    const double maxEpsilon = 0.3 * peri;  // Upper limit
    const double increment = 0.01 * peri;  // Increment value

    while (epsilon <= maxEpsilon) {
        cv::approxPolyDP(docContour, approx, epsilon, true);
        if (approx.size() == 4) {
            displayContour = approx;
            break;
        }
        epsilon += increment;
    }

    if (displayContour.empty()) {
        std::cout << "No four-vertex contour found!" << std::endl;
        return;
    }

    cv::Point2f bottomRight = displayContour[2];

    cv::Rect rect(cv::Point2f(0,0), bottomRight);

    displayContour = {
        cv::Point2f(0, 0),
        cv::Point2f(bottomRight.x, 0),
        cv::Point2f(bottomRight.x, bottomRight.y),
        cv::Point2f(0, bottomRight.y)
    };

    // Now, scale the displayContour points back to original dimensions
    for (cv::Point& pt : displayContour) {
        pt.x *= 2; // Since you resized by 0.5, you multiply by 2 to scale back
        pt.y *= 2;
    }

    // Apply the fourPointTransform on the original image using the scaled contour points
    cv::Mat warped = fourPointTransform(originalImage, displayContour);

    // Draw all the contours on the image
    cv::Mat contourImage = image.clone();
    cv::drawContours(contourImage, contours, -1, cv::Scalar(0, 255, 0), 2);

    // save image
    cv::imwrite("test/test_output.png", warped);
    // Save the image with contours
    cv::imwrite("test/contours_output.png", contourImage);

    // Visualize the image with contours
    cv::imshow("Contours Image", warped);

    // // Visualize the result
    // cv::imshow("Warped Image", warped);
    cv::waitKey(0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
