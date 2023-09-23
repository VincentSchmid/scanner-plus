#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <hpdf.h>

#include "ScannedPage.h"

class ScannedDocument {
public:
    ScannedDocument();
    void addPage(ScannedPage page);
    std::vector<ScannedPage>& getPages();

    void savePdf(std::string filename);
    std::vector<uchar> getPdfData();

private:
    std::vector<ScannedPage> pages;

    float mmToPoint(float mm);
    void addImageToPdf(HPDF_Doc pdf, const std::vector<uchar>& img, float imgWidth, float imgHeight);
};
