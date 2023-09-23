#include "scanner/DocumentScanner.h"

DocumentScanner::DocumentScanner() {
    // Initialize the scanner hardware.
}

ScannedPage DocumentScanner::scan(const std::string device_name, int dpi, int width_mm, int height_mm) {
    cv::Mat imageData = scan_document(device_name.c_str(), dpi, width_mm, height_mm);
    std::vector<uchar> jpgBuffer = get_jpeg_buffer(imageData);
    return ScannedPage(jpgBuffer, width_mm, height_mm);
}
