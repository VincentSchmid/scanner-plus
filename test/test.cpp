#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

#include "scanner/DocumentScanner.h"
#include "scanner/ScannedPage.h"
#include "scanner/TestScanner.h"
#include "scanner/SaneScanner.h"
#include "scanner/imageProcessing.h"


// Sample test case (modify as needed)
TEST(DocumentScannerTest, DISABLED_TestScanFunction) {
    auto saneScanner = std::make_unique<SaneScanner>();
    DocumentScanner scanner = DocumentScanner(std::move(saneScanner));

    int dpi = 150;
    const char *device_name = "pixma:04A91908";
    ScannedPage page = scanner.scan(device_name, dpi);
}

TEST(CropTest, DISABLED_TestImgCrop) {
    cv::Mat image = cv::imread("testdata/Impfausweis-1.png");
    cv::Mat contourImage = image.clone();
    std::vector<cv::Point2f> singleContour = getDocumentContours(image);
    std::vector<std::vector<cv::Point2f>> contours = { singleContour };
    //image = crop_image(image, contours);

    cv::drawContours(contourImage, contours, -1, cv::Scalar(0, 255, 0), 2);

    cv::imwrite("result/image-contours.jpg", contourImage);
}

TEST(CropTest, TestImgCrop2) {
    cv::Mat image = cv::imread("testdata/scan5.pnm");
    cv::Mat contourImage = image.clone();
    
    cv::Mat smallImage;

    // Resize the smallImage
    cv::resize(image, smallImage, cv::Size(), 0.5, 0.5);

    // Create a mask based on the known background color
    cv::Mat mask;
    cv::Mat gray;
    cv::cvtColor(smallImage, gray, cv::COLOR_BGR2GRAY);
    cv::adaptiveThreshold(gray, mask, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 2);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(9, 9));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    // Visualize the mask after closing
    cv::imshow("Image", mask);
    cv::waitKey(0);

    // Erode the entire mask
    cv::Mat erodedMask;
    cv::erode(mask, mask, kernel, cv::Point(-1, -1), 1);
    cv::dilate(mask, mask, kernel, cv::Point(-1, -1), 1);

    cv::imshow("Image", mask);
    cv::waitKey(0);

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

    // show me

    std::vector<std::vector<cv::Point>> contourys = { docContour };

    cv::Mat smallImageCopy = smallImage.clone();
    //cv::drawContours(contourImage, contours, -1, cv::Scalar(0, 255, 0), 2);
    cv::drawContours(smallImageCopy, contourys, -1, cv::Scalar(0, 255, 0), 2);

    cv::imshow("Image", smallImageCopy);
    cv::waitKey(0);

    // endshow

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

    // get largest x, y value from displayContour
    cv::Point2f bottomRight;
    
    for (cv::Point& pt : displayContour) {
        if (pt.x > bottomRight.x) {
            bottomRight.x = pt.x;
        }
        if (pt.y > bottomRight.y) {
            bottomRight.y = pt.y;
        }
    }

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

    std::vector<std::vector<cv::Point>> contours2 = { displayContour };

    cv::drawContours(contourImage, contours2, -1, cv::Scalar(0, 255, 0), 2);

    cv::imshow("Image", contourImage);
    // cv::imwrite("result/image-contours.jpg", contourImage);
    cv::waitKey(0);

    image = adjustBrightnessContrast(image, -50.0, 18.0);

    cv::imshow("Image", image);
    cv::waitKey(0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


