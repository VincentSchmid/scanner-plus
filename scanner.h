#pragma once

extern "C"
{
#include <sane/sane.h>
}

#ifndef SCANNER_H
#define SCANNER_H

const cv::Mat scan_document(const char *device_name, SANE_Int dpi, int doc_width_mm, int doc_height_mm);
std::vector<uchar> get_jpeg_buffer(cv::Mat img);
const cv::Mat image_processing(cv::Mat img);
#endif // SCANNER_H
