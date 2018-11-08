#pragma once

#include "my_utils_kk4.hpp"

#include <opencv2/opencv.hpp>

#include <string>

class WindowManager{

public:

    typedef enum{
        NOTHING,
        NEXT_IM,
        PREVIOUS_IM,
        UPPER_DIR,
        LOWER_DIR,
        NEXT_DIR,
        PREVIOUS_DIR
    }Command;
    
    WindowManager();
    WindowManager(const std::string& window_name);
    ~WindowManager();

    void update(const cv::Mat& im, const std::string& current_path);
    Command nextCommand()const;
    bool isShutdown()const;

protected:

    std::string window_name;
};


