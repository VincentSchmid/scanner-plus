#pragma once

#include <vector>
#include <opencv2/core/core.hpp>

class ScannedPage {
public:
    ScannedPage(std::vector<uchar> imageData, int width_mm, int height_mm);
    const std::vector<uchar>& getImageData() const;
    int getWidth() const;
    int getHeight() const;

private:
    std::vector<uchar> imageData;
    int width_mm;
    int height_mm;
};
