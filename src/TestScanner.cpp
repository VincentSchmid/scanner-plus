#include "scanner/TestScanner.h"

TestScanner::TestScanner(const cv::Mat& inputImage) : outputImg(inputImage) {}

void TestScanner::scanDocument(const char* device_name, int dpi, int width_mm, int height_mm, cv::Mat& imageData) {
    imageData = outputImg;
}
