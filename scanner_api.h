#ifndef SCANNER_API_H
#define SCANNER_API_H

typedef struct {
    unsigned char* data;
    unsigned long size;
} PdfData;

void* create_document();
void add_page(void* doc, const char* device_name, int dpi, int doc_width_mm, int doc_height_mm);
PdfData get_pdf_document(void* doc_ptr);
void free_pdf_data(PdfData data);

#endif