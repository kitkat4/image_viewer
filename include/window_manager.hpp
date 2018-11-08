#pragma once

#include <opencv2/opencv.hpp>

#include <string>

class WindowManager{

public:
    
    WindowManager();
    WindowManager(const std::string& window_name);
    ~WindowManager();

    void update(const cv::Mat& im, const std::string& current_path);
    bool isShutdown()const;

protected:

    std::string window_name;
};


