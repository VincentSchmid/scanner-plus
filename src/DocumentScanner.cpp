#include "scanner/DocumentScanner.h"

ScannedPage DocumentScanner::scan(const std::string device_name, int dpi, int width_mm, int height_mm) {
    cv::Mat imageData;
    scannerHardware->scanDocument(device_name.c_str(), dpi, width_mm, height_mm, imageData);
    std::vector<cv::Point2f> contours = getDocumentContours(imageData);
    imageData = crop_image(imageData, contours);
    imageData = adjustBrightnessContrast(imageData, -50.0, 18.0);

    std::vector<uchar> jpgBuffer = get_jpeg_buffer(imageData);
    return ScannedPage(jpgBuffer, width_mm, height_mm);
}
