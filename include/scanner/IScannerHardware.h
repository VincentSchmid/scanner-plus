#ifndef ISCANNERHARDWARE_H
#define ISCANNERHARDWARE_H

#include <opencv2/opencv.hpp>

class IScannerHardware {
public:
    virtual ~IScannerHardware() = default;  // virtual destructor
    virtual void scanDocument(const char* device_name, int dpi, int width_mm, int height_mm, cv::Mat& imageData) = 0;
};

#endif
