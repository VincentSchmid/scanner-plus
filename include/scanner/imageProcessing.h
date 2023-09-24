#pragma once

#ifndef IMAGE_SHARPENING_H
#define IMAGE_SHARPENING_H

#include <opencv2/opencv.hpp>

std::vector<uchar> get_jpeg_buffer(cv::Mat img);
const cv::Mat sharpen_image(cv::Mat img);
const cv::Mat auto_crop_image(cv::Mat img);
std::vector<cv::Point2f> order_points(const std::vector<cv::Point>& pts);
cv::Mat fourPointTransform(const cv::Mat& image, const std::vector<cv::Point>& pts);

#endif
