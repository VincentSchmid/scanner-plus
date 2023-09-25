#ifndef SaneScanner_H
#define SaneScanner_H

#include "IScannerHardware.h"

class SaneScanner : public IScannerHardware {
public:
    void scanDocument(const char* device_name, int dpi, int width_mm, int height_mm, cv::Mat& imageData) override;
};

#endif
