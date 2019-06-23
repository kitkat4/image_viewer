#pragma once

#include "my_utils_kk4.hpp"

#include <opencv2/opencv.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>


#include <string>
#include <cmath>

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
        SCALE_UP,
        SCALE_DOWN,
        QUIT
    }Command;
    
    WindowManager();
    WindowManager(const std::string& window_name, const int width, const int height);
    ~WindowManager();

    void update(const cv::Mat& im, const std::string& current_path);
    Command nextCommand()const;
    bool isShutdown()const;
    void scaleUp();
    void scaleDown();
    void closeWindow();

protected:

    std::string window_name;

    void drawImage(const cv::Mat& im);
    void generateImageToDrawFitToWindow(const cv::Mat& in_im, cv::Mat * const out_im,
                                        int * const upper_left_x, int * const upper_left_y,
                                        double * const scale)const;
    void generateImageToDraw(const cv::Mat& im, cv::Mat * const out_im,
                             int * const upper_left_x, int * const upper_left_y,
                             double * const scale)const;
    double calcScaleToFitToWindow(const cv::Mat& im)const;
    void getWindowSize(int * const width, int * const height)const;
    void setDefaultBackground();
    void getRegionToDraw(int * const upper_left_x, int * const upper_left_y,
                         int * const lower_right_x, int * const lower_right_y);

    Display * dis;
    int screen;
    Window win;
    GC gc;

    // Must be odd number.
    const int sliding_step_num;
    int cur_offset_x;           // in pixel
    int cur_offset_y;           // in pixel
    
    // scale = initial_scale * pow(scale_base, cur_scale_exponent)
    double initial_scale;
    const double scale_base;
    int cur_scale_exponent;
    double last_scale;

    // If true, scale is ignored.
    bool fit_to_window;

    int tile_size;
};


