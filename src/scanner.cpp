extern "C"
{
#include <sane/sane.h>
}

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#include "scanner/scanner.h"


// Actual implementation of the scan_document function
const cv::Mat scan_document(const char *device_name, SANE_Int dpi, int doc_width_mm, int doc_height_mm)
{
    SANE_Status status;
    SANE_Handle device;
    SANE_Int version;
    SANE_Parameters params;

    // Initialize sane
    status = sane_init(&version, NULL);
    if (status != SANE_STATUS_GOOD)
    {
        // Error handling
    }

    // Open device (device_name passed as an argument)
    status = sane_open(device_name, &device);
    if (status != SANE_STATUS_GOOD)
    {
        // Error handling
    }

    // Adjust the scanning parameters for the document
    params.format = SANE_FRAME_RGB;
    params.last_frame = 0;
    params.pixels_per_line = doc_width_mm * dpi / 25.4; // width in pixels
    params.lines = doc_height_mm * dpi / 25.4;          // height in pixels
    params.depth = 8;                                   // 8 bits per channel (assuming RGB format)
    params.bytes_per_line = params.pixels_per_line * 3; // Assuming RGB format (3 bytes per pixel)

    SANE_Int num_options;
    sane_control_option(device, 0, SANE_ACTION_GET_VALUE, &num_options, 0);

    SANE_Int resolution_option_index = -1;

    for (SANE_Int i = 0; i < num_options; i++)
    {
        const SANE_Option_Descriptor *opt = sane_get_option_descriptor(device, i);
        if (opt == NULL)
            continue;

        if (opt->name != NULL && strcmp(opt->name, "resolution") == 0)
        {
            resolution_option_index = i;
            break;
        }
    }

    if (resolution_option_index != -1)
    {
        // Set DPI value passed as an argument
        status = sane_control_option(device, resolution_option_index, SANE_ACTION_SET_VALUE, &dpi, NULL);
        if (status != SANE_STATUS_GOOD)
        {
            // Error handling
        }
    }
    else
    {
        std::cout << "Resolution option not found" << std::endl;
    }

    // Set the mode to color
    const char *mode = "Color";
    status = sane_control_option(device, 1, SANE_ACTION_SET_VALUE, (void *)mode, NULL);
    if (status != SANE_STATUS_GOOD)
    {
        // Error handling
    }

    // Start scanning
    status = sane_start(device);
    if (status != SANE_STATUS_GOOD)
    {
        // Error handling
    }

    // Get scan parameters
    status = sane_get_parameters(device, &params);
    if (status != SANE_STATUS_GOOD)
    {
        // Error handling
    }

    // Read the image data
    std::vector<uint8_t> image_data;
    SANE_Int read_length;
    do
    {
        std::vector<uint8_t> buffer(params.bytes_per_line);
        status = sane_read(device, buffer.data(), buffer.size(), &read_length);
        if (status != SANE_STATUS_GOOD && status != SANE_STATUS_EOF)
        {
            // Error handling
        }

        // Append the buffer to the image data
        image_data.insert(image_data.end(), buffer.begin(), buffer.begin() + read_length);
    } while (status == SANE_STATUS_GOOD);

    // Close the device and exit SANE
    sane_close(device);
    sane_exit();

    // Convert the image data to an OpenCV Mat
    cv::Mat img(params.lines, params.pixels_per_line, CV_8UC3, image_data.data());

    // Reorder color channels to RGB
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

    return image_processing(img);
}

std::vector<uchar> get_jpeg_buffer(cv::Mat img)
{
    std::vector<uchar> buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(90); // Quality level

    cv::imencode(".jpg", img, buffer, params);

    return buffer;
}
