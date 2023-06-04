extern "C" {
    #include <sane/sane.h>
}

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#include "scanner.h"

int main() {
    const char *device_name = "pixma:04A91908"; // Replace this with your actual device name
    SANE_Int dpi = 300; // Desired DPI
    int doc_width_mm = 210; // Document width in mm (A4 paper)
    int doc_height_mm = 297; // Document height in mm (A4 paper)

    const char* filename = scan_document(device_name, dpi, doc_width_mm, doc_height_mm);
    std::cout << "Scanned document saved to " << filename << std::endl;
    return 0;
}
