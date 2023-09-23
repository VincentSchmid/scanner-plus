#include <gtest/gtest.h>
#include "DocumentScanner.h"  // Include relevant headers for your testing

// Sample test case (modify as needed)
TEST(DocumentScannerTest, TestScanFunction) {
    // Arrange
    DocumentScanner scanner;
    
    // Act
    // Assuming scan function returns bool for success
    bool result = scanner.scan("sample_image.jpg", 300, 300, 300);  // Replace with actual arguments
    
    // Assert
    EXPECT_TRUE(result);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
