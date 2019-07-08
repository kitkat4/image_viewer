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
public:
    ImageViewer(const std::string& path);
    ~ImageViewer();
    void enterMainLoop();

protected:

    void update();
    
    std::unique_ptr<DirScanner> dir_scanner;
    std::unique_ptr<WindowManager> wm;
    cv::Mat cur_im;
    
    std::string cur_path;
};

