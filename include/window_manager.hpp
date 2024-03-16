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
        NEXT_BROTHER_DIR,
        PREVIOUS_DIR,
        PREVIOUS_BROTHER_DIR,
        NEXT_PARENT,
        PREVIOUS_PARENT,
        REDRAW,
        SCALE_UP,
        SCALE_DOWN,
        MOVE_RIGHT,
        MOVE_LEFT,
        MOVE_UP,
        MOVE_DOWN,
        // MOVE_CENTER,
        CLEAR,
        QUIT
    }Command;
    
    WindowManager();
    WindowManager(const int width, const int height);
    ~WindowManager();

    void update(const cv::Mat& im, const std::string& current_path);
    Command nextCommand();
    void scaleUp();
    void scaleDown();
    void moveRight();
    void moveLeft();
    void moveUp();
    void moveDown();
    void moveCenter();
    void clearScaleAndOffset();
    void closeWindow();

protected:

    void drawImage(const cv::Mat& im);
    void generateImageToDrawFitToWindow(const cv::Mat& in_im, cv::Mat * const out_im,
                                        double * const upper_left_x, double * const upper_left_y,
                                        double * const scale)const;
    void generateImageToDraw(const cv::Mat& im, cv::Mat * const out_im,
                             double * const upper_left_x, double * const upper_left_y,
                             double * const scale)const;
    
    double calcScaleToFitToWindow(const cv::Mat& im)const;
    void getWindowSize(int * const width, int * const height)const;
    void setDefaultBackground();

    // To get the coordinates of upper left and lower right point of the region
    // to draw. The values are represented in the image coordinates.
    void getRegionToDraw(const cv::Mat& in_im, const double scale,
                         double * const upper_left_x, double * const upper_left_y,
                         double * const lower_right_x, double * const lower_right_y)const;
    Command processEvent(const XEvent& event);
    std::string generateWindowTitleFromPathString(const std::string& path_str)const;

    bool isShiftPressed()const;
    bool isCtrlPressed()const;
    void disableFitToWindow();

    void window2ImageCoord(const int x_window, const int y_window,
                           int * const x_image, int * const y_image)const;

    Display * dis;
    int screen;
    Window win;
    GC gc;

    bool shift_l_pressed, shift_r_pressed;
    bool ctrl_l_pressed, ctrl_r_pressed;
    bool left_dragging;
    bool maybe_left_click;
    int last_x_while_dragging;
    int last_y_while_dragging;

    const size_t max_queue_size;

    // Must be odd number.
    const int sliding_step;     // in pixel of the window
    double cur_offset_x;           // in pixel of the window
    double cur_offset_y;           // in pixel of the window
    
    // scale = initial_scale * pow(scale_base, cur_scale_exponent)
    double initial_scale;
    const double scale_base;
    int cur_scale_exponent;
    double last_scale;

    // If true, scale is ignored.
    bool fit_to_window;

    const int tile_size;

    cv::Mat last_im;
};


