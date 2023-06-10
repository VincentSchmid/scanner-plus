#include "ScannedPage.h"

ScannedPage::ScannedPage(std::vector<uchar> imageData, int width_mm, int height_mm)
    : imageData(std::move(imageData)), width_mm(width_mm), height_mm(height_mm) {}

const std::vector<uchar>& ScannedPage::getImageData() const {
    return imageData;
}

int ScannedPage::getWidth() const {
    return width_mm;
}

int ScannedPage::getHeight() const {
    return height_mm;
}
