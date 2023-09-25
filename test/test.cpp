#include <gtest/gtest.h>
#include <opencv2/opencv.hpp>

#include "scanner/DocumentScanner.h"
#include "scanner/ScannedPage.h"
#include "scanner/TestScanner.h"
#include "scanner/SaneScanner.h"


// Sample test case (modify as needed)
TEST(DocumentScannerTest, DISABLED_TestScanFunction) {
    auto saneScanner = std::make_unique<SaneScanner>();
    DocumentScanner scanner = DocumentScanner(std::move(saneScanner));

    int dpi = 150;
    const char *device_name = "pixma:04A91908"; // Replace this with your actual device name
    int doc_width_mm = 210; // Document width in mm (A4 paper)
    int doc_height_mm = 297; // Document height in mm (A4 paper)
    ScannedPage page = scanner.scan(device_name, dpi, doc_width_mm, doc_height_mm);
}

TEST(CropTest, TestImgCrop) {
    cv::Mat image = cv::imread("testdata/scan.pnm");

    image = auto_crop_image(image);

    cv::imwrite("result/scan-cropped.jpg", image);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
