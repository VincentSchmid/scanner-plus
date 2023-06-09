#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#include "DocumentScanner.h"
#include "ScannedDocument.h"
#include "ScannedPage.h"
#include "scanner.h"

int main() {
    const char *device_name = "pixma:04A91908"; // Replace this with your actual device name
    int dpi = 300; // Desired DPI
    int doc_width_mm = 210; // Document width in mm (A4 paper)
    int doc_height_mm = 297; // Document height in mm (A4 paper)

    DocumentScanner scanner;
    ScannedDocument doc;
    ScannedPage page = scanner.scan(device_name, dpi, doc_width_mm, doc_height_mm);
    
    doc.addPage(page);
    doc.addPage(page);

    doc.savePdf("test2.pdf");

    return 0;
}
