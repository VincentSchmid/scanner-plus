#pragma once

extern "C"
{
#include <sane/sane.h>
}

#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <opencv2/opencv.hpp>

const cv::Mat scan_document(const char *device_name, SANE_Int dpi, int doc_width_mm, int doc_height_mm);
#endif // SCANNER_H
