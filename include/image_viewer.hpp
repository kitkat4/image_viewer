#pragma once


#include <opencv2/opencv.hpp>

#include "dir_scanner.hpp"
#include "my_utils_kk4.hpp"
#include "window_manager.hpp"

#include <memory>
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

    void update();
    
    const std::string window_name;
    bool window_opened;
    DirScanner* dir_scanner;
    std::unique_ptr<WindowManager> wm;
    cv::Mat cur_im;
    cv::Mat cur_im_original;
    std::string cur_path;
};

