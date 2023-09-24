#include <gtest/gtest.h>
#include "scanner/DocumentScanner.h"
#include "scanner/ScannedPage.h"


// Sample test case (modify as needed)
TEST(DocumentScannerTest, TestScanFunction) {
    DocumentScanner scanner;
    int dpi = 150;
    const char *device_name = "pixma:04A91908"; // Replace this with your actual device name
    int doc_width_mm = 210; // Document width in mm (A4 paper)
    int doc_height_mm = 297; // Document height in mm (A4 paper)
    ScannedPage page = scanner.scan(device_name, dpi, doc_width_mm, doc_height_mm);

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
