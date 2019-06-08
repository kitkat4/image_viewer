#pragma once

#include "my_utils_kk4.hpp"

#include <opencv2/opencv.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>


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
        PREVIOUS_DIR,
        REDRAW,
        QUIT
    }Command;
    
    WindowManager();
    WindowManager(const std::string& window_name, const int width, const int height);
    ~WindowManager();

    void update(const cv::Mat& im);
    Command nextCommand()const;
    bool isShutdown()const;
    void closeWindow();

protected:

    std::string window_name;

    void drawImage(const cv::Mat& im);

    Display * dis;
    int screen;
    Window win;
    GC gc;
};


