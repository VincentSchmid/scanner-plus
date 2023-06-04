#include <vector>
#include <opencv2/core/core.hpp>

class ScannedPage {
public:
    ScannedPage(cv::Mat imageData)
        : imageData(std::move(imageData)) {}
    const cv::Mat& getImageData() const { return imageData; }

private:
    cv::Mat imageData;
};

class ScannedDocument {
public:
    void addPage(ScannedPage page) { pages.push_back(std::move(page)); }
    std::vector<ScannedPage>& getPages() { return pages; }

    // Combine pages into a single file (pseudo code)
    void combinePages() {
        HPDF_Doc pdf = HPDF_New(NULL, NULL);
        for (const ScannedPage& page : pages) {
            addImageToPdf(pdf, page.getImageData());
        }
        HPDF_SaveToFile(pdf, "output.pdf");
        HPDF_Free(pdf);
    }

private:
    std::vector<ScannedPage> pages;
};
