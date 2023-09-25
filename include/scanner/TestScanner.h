#ifndef TESTSCANNER_H
#define TESTSCANNER_H

#include <string>
#include <opencv2/opencv.hpp>
#include "scanner/IScannerHardware.h"

class TestScanner : public IScannerHardware {
private:
    cv::Mat outputImg;

public:
    TestScanner(const cv::Mat& inputImage);
    void scanDocument(const char* device_name, int dpi, int width_mm, int height_mm, cv::Mat& imageData) override;
};

#endif
