#pragma once

#include "my_utils_kk4.hpp"

#include <opencv2/opencv.hpp>

#ifdef IMAGE_VIEWER_WITH_X11_WINDOW

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>

#endif


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

#ifdef IMAGE_VIEWER_WITH_X11_WINDOW

    Display * dis;
    int screen;
    Window win;
    GC gc;
    
    
#endif

};


