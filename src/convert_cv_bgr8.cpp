/*
 *  Author: Maarten Vandersteegen EAVISE
 *  Description: Image converter to convert an opencv BGR 8-bit image to a darknet image
 */

#include "convert_cv_bgr8.hpp"
#include "logging.hpp"

using namespace Darknet;

void ConvertCvBgr8::setup(int in_width, int in_height, int out_width, int out_height)
{
    int new_w;
    int new_h;

    m_in_width = in_width;
    m_in_height = in_height;
    m_out_width = out_width;
    m_out_height = out_height;

    m_resize_needed = m_in_width != m_out_width || m_in_height != m_out_height;

    /* calculate a width - height pair that fits inside out_width - out_height
       and has the same aspect ratio of in_width - in_height */
   if (((float)out_width/in_width) < ((float)out_height/in_height)) {
        new_w = out_width;
        new_h = (in_height * out_width) / in_width;
    } else {
        new_h = out_height;
        new_w = (in_width * out_height) / in_height;
    }

    m_image_resize = cv::Mat(out_height, out_width, CV_8UC3);
    m_image_resize_roi = cv::Mat(m_image_resize, cv::Rect((out_width - new_w) / 2, (out_height - new_h) / 2, new_w, new_h));
    m_new_size.width = new_w;
    m_new_size.height = new_h;
}

bool ConvertCvBgr8::convert(const cv::Mat& in, Image& out)
{
    std::vector<cv::Mat> floatMatChannels(3);

    if (in.empty() || in.channels() != 3) {
        EPRINTF("Input image empty or wrong number of image channels\n");
        return false;
    }

    if (m_resize_needed) {
        cv::resize(in, m_image_resize_roi, m_new_size);
        cv::cvtColor(m_image_resize, m_image_rgb, cv::COLOR_BGR2RGB);
    } else {
        cv::cvtColor(in, m_image_rgb, cv::COLOR_BGR2RGB);
    }

    // ensure out is allocated
    out.resize(m_out_width, m_out_height, 3);
    floatMatChannels[0] = cv::Mat(m_out_height, m_out_width, CV_32FC1, &out.data[0]);
    floatMatChannels[1] = cv::Mat(m_out_height, m_out_width, CV_32FC1, &out.data[m_out_width * m_out_height]);
    floatMatChannels[2] = cv::Mat(m_out_height, m_out_width, CV_32FC1, &out.data[2 * m_out_width * m_out_height]);

    // Convert the bytes to float
    m_image_rgb.convertTo(m_image_rgb_float, CV_32FC3, 1/255.0);

    // Concatenate R,G,B channels in one big buffer
    cv::split(m_image_rgb_float, floatMatChannels);

    return true;
}
