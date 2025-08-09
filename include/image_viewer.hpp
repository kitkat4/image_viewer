#pragma once


#include <opencv2/opencv.hpp>

#include "dir_scanner.hpp"
#include "window_manager.hpp"

#include <memory>
#include <string>
#include <iostream>

class DirScanner;

class ImageViewer{
public:
    ImageViewer();
    ~ImageViewer();
    bool init(const std::string& path);
    void enterMainLoop();

protected:

    void update();
    void showHelp()const;
    
    std::unique_ptr<DirScanner> dir_scanner;
    std::unique_ptr<WindowManager> wm;
    cv::Mat cur_im;
    
    std::string cur_path;

    const int skip_count;
};

