#pragma once

#include <vector>
#include <opencv2/core/core.hpp>


class ScannedPage {
public:
    ScannedPage(std::vector<uchar> imageData, int width_mm, int height_mm)
        : imageData(std::move(imageData)), width_mm(width_mm), height_mm(height_mm) {}
    const std::vector<uchar>& getImageData() const { return imageData; }
    int getWidth() const { return width_mm; }
    int getHeight() const { return height_mm; }

private:
    std::vector<uchar> imageData;
    int width_mm;
    int height_mm;
};
