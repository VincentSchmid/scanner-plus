#include "scanner/SaneScanner.h"
#include "scanner/scanner.h"


void SaneScanner::scanDocument(const char* device_name, int dpi, int width_mm, int height_mm, cv::Mat& imageData) {
    // Your real hardware scanning logic here.
    scan_document(device_name, dpi, width_mm, height_mm, imageData);
}
