#pragma once

#include "scanner.h"
#include <opencv2/opencv.hpp>

#include "ScannedPage.h"
#include "imageProcessing.h"
#include "IScannerHardware.h"

class DocumentScanner {
private:
    std::unique_ptr<IScannerHardware> scannerHardware;
public:
    DocumentScanner(std::unique_ptr<IScannerHardware> hardware) 
        : scannerHardware(std::move(hardware)) {};

    ScannedPage scan(const std::string device_name, int dpi, int width_mm, int height_mm);
};
