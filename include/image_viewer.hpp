#pragma once


#ifdef IMAGE_VIEWER_WITH_X11_WINDOW

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#endif

#include "dir_scanner.hpp"

#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>

class DirScanner;

class ImageViewer{
    friend class DirScanner;
public:
    ImageViewer(const std::string& path, const std::string& window_name = "Image Viewer");
    ~ImageViewer();
    void enterMainLoop();

protected:
    const std::string window_name;
    bool window_opened;
    DirScanner* cur_instance;
    cv::Mat cur_im;
    cv::Mat cur_im_original;
    std::string cur_path;
};

