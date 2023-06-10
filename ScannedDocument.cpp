#include "ScannedDocument.h"


ScannedDocument::ScannedDocument() {
}


void ScannedDocument::addPage(ScannedPage page) {
    pages.push_back(std::move(page));
}

std::vector<ScannedPage>& ScannedDocument::getPages() {
    return pages;
}

void ScannedDocument::savePdf(std::string filename) {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    for (const ScannedPage& page : pages) {
        addImageToPdf(pdf, page.getImageData(), page.getWidth(), page.getHeight());
    }
    HPDF_STATUS status = HPDF_SaveToFile(pdf, filename.c_str());

    if (status != HPDF_OK) {
        std::cerr << "Error saving PDF file: " << status << std::endl;
    }
    HPDF_Free(pdf);
}

std::vector<uchar> ScannedDocument::getPdfData() {
    HPDF_Doc pdf = HPDF_New(NULL, NULL);
    for (const ScannedPage& page : pages) {
        addImageToPdf(pdf, page.getImageData(), page.getWidth(), page.getHeight());
    }
    HPDF_SaveToStream(pdf);
    HPDF_ResetStream(pdf);

    HPDF_UINT32 size = HPDF_GetStreamSize(pdf);
    std::vector<uchar> data(size);
    HPDF_ReadFromStream(pdf, data.data(), &size);
    HPDF_Free(pdf);

    return data;
}

float ScannedDocument::mmToPoint(float mm) {
    return mm * 2.83465; // conversion factor from mm to points (1 point = 1/72 inches = ~2.83465 mm)
}

void ScannedDocument::addImageToPdf(HPDF_Doc pdf, const std::vector<uchar>& img, float imgWidth, float imgHeight) {
    // Convert dimensions from mm to points
    float imgWidthPt = mmToPoint(imgWidth);
    float imgHeightPt = mmToPoint(imgHeight);

    // Create the HPDF image object
    HPDF_Image image = HPDF_LoadJpegImageFromMem(pdf, img.data(), img.size());

    // Create a new page and draw the image on it
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetWidth(page, imgWidthPt);
    HPDF_Page_SetHeight(page, imgHeightPt);
    HPDF_Page_DrawImage(page, image, 0, 0, imgWidthPt, imgHeightPt);
}
