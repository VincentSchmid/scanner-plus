#pragma once

#include "scanner.h"
#include <opencv2/opencv.hpp>

#include "ScannedPage.h"

class DocumentScanner {
public:
    DocumentScanner();

    ScannedPage scan(const std::string device_name, int dpi, int width_mm, int height_mm);
};
