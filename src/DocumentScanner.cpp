#include "scanner/DocumentScanner.h"

ScannedPage DocumentScanner::scan(const std::string device_name, int dpi) {
    cv::Mat imageData;
    scannerHardware->scanDocument(device_name.c_str(), dpi, 210, 297, imageData);
    std::vector<cv::Point2f> contours = getDocumentContours(imageData);
    cv::Mat croppedImage = crop_image(imageData, contours);
    cv::Mat result = adjustBrightnessContrast(croppedImage, -50.0, 18.0);

    double width_mm = result.cols * 25.4 / dpi;
    double height_mm = result.rows * 25.4 / dpi;

    std::vector<uchar> jpgBuffer = get_jpeg_buffer(result);
    return ScannedPage(jpgBuffer, width_mm, height_mm);
}
