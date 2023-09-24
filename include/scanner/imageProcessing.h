#pragma once

#ifndef IMAGE_SHARPENING_H
#define IMAGE_SHARPENING_H

#include <opencv2/opencv.hpp>

std::vector<uchar> get_jpeg_buffer(cv::Mat img);
const cv::Mat sharpen_image(cv::Mat img);
const cv::Mat auto_crop_image(cv::Mat img);

#endif
