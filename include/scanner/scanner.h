#pragma once

#ifndef SCANNER_H
#define SCANNER_H

extern "C"
{
#include <sane/sane.h>
}

#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

const void scan_document(const char *device_name, SANE_Int dpi, int doc_width_mm, int doc_height_mm, cv::Mat& outputImg);
#endif // SCANNER_H
