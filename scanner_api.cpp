#include "scanner_api.h"
#include "DocumentScanner.h"
#include "ScannedDocument.h"
#include "ScannedPage.h"
#include <map>

static std::map<void*, ScannedDocument> doc_map;

void* create_document() {
    ScannedDocument* doc_ptr = new ScannedDocument();
    doc_map[doc_ptr] = ScannedDocument();
    return doc_ptr;
}

void add_page(void* doc_ptr, const char* device_name, int dpi, int doc_width_mm, int doc_height_mm) {
    DocumentScanner scanner;
    ScannedPage page = scanner.scan(device_name, dpi, doc_width_mm, doc_height_mm);
    doc_map[doc_ptr].addPage(page);
}

PdfData get_pdf_document(void* doc_ptr) {
    std::vector<unsigned char> data = doc_map[doc_ptr].getPdfData();
    unsigned char* data_ptr = new unsigned char[data.size()];
    std::copy(data.begin(), data.end(), data_ptr);

    PdfData pdfData;
    pdfData.data = data_ptr;
    pdfData.size = data.size();

    return pdfData;
}

void free_pdf_data(PdfData data) {
    delete[] data.data;
}
