#include "window_manager.hpp"

WindowManager::WindowManager():
    sliding_step_num(21),
    cur_offset_x(0),            // center
    cur_offset_y(0),            // center
    initial_scale(1.0),
    scale_base(2.0), cur_scale_exponent(0),
    last_scale(1.0), fit_to_window(true), tile_size(64)
{

}

WindowManager::WindowManager(const std::string& window_name, const int width, const int height)
    :window_name(window_name), sliding_step_num(21),
     cur_offset_x(0),           // center
     cur_offset_y(0),           // center
     initial_scale(1.0),
     scale_base(2.0), cur_scale_exponent(0),
     last_scale(1.0), fit_to_window(true), tile_size(64){


    dis = XOpenDisplay(nullptr);

    if(! dis){
        std::cerr << my_utils_kk4::red
                  << "[ERROR] Failed to open a new window (" << __FILE__
                  << ", line " << __LINE__ << ")"
                  << my_utils_kk4::default_color
                  << std::endl;
    }
    
    screen = DefaultScreen(dis);

    win = XCreateSimpleWindow(dis, RootWindow(dis, screen), 0, 0, width, height,
                              5, None, None);

    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask);

    gc = XCreateGC(dis, win, 0, 0);

    setDefaultBackground();
    
    XMapRaised(dis, win);

    while(true){
        XEvent x_event;
        XNextEvent(dis, &x_event);
        if(x_event.type == Expose){
            XClearWindow(dis, win);
            break;
        }
    }
}

WindowManager::~WindowManager(){}

void WindowManager::update(const cv::Mat& im, const std::string& current_path){

    XStoreName(dis, win, current_path.c_str());

    XChangeProperty(dis, win,
                    XInternAtom(dis, "_NET_WM_NAME", False),
                    XInternAtom(dis, "UTF8_STRING", False),
                    8, PropModeReplace, (const unsigned char*)current_path.c_str(),
                    strlen(current_path.c_str()));
    
    if(im.empty()){
        XClearWindow(dis, win);
    }else{
        drawImage(im);
    }
}

WindowManager::Command WindowManager::nextCommand()const{

    XEvent x_event;
    
    XNextEvent(dis, &x_event);

    if(x_event.type == KeyPress){
        
        KeySym key_sym = XkbKeycodeToKeysym(dis, x_event.xkey.keycode, 0,
                                            x_event.xkey.state & ShiftMask ? 1 : 0);
        
        switch(key_sym){
        case XK_Right:
            return NEXT_IM;
        case XK_Left:
            return PREVIOUS_IM;
        case XK_Up:
            return UPPER_DIR;
        case XK_Down:
            return LOWER_DIR;
        case XK_Page_Up:
            return PREVIOUS_DIR;
        case XK_Page_Down:
            return NEXT_DIR;
        case XK_plus:
            return SCALE_UP;
        case XK_minus:
            return SCALE_DOWN;
        case XK_q:
            return QUIT;
        default:
            return NOTHING;
        }
        
    }else if(x_event.type == ConfigureNotify){
        return REDRAW;
    }else{
        return NOTHING;
    }
    
}

bool WindowManager::isShutdown()const{

    // TODO! 
    // check whether the window is closed
    return false;
}

void WindowManager::scaleUp(){

    if(fit_to_window){
        fit_to_window = false;
        initial_scale = last_scale;
    }
    
    cur_scale_exponent += 1;
}

void WindowManager::scaleDown(){

    if(fit_to_window){
        fit_to_window = false;
        initial_scale = last_scale;
    }
    
    cur_scale_exponent -= 1;
}

void WindowManager::closeWindow(){

    XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);	
    
}


void WindowManager::drawImage(const cv::Mat& im){

    XColor c;
    cv::Vec3b v;
    Colormap cmap = XDefaultColormap(dis, screen);

    const int depth = DefaultDepth(dis, screen);

    int upper_left_x, upper_left_y;

    cv::Mat tmp_im;
    
    if(fit_to_window){
        generateImageToDrawFitToWindow(im, &tmp_im, &upper_left_x, &upper_left_y, &last_scale);
    }else{
        generateImageToDraw(im, &tmp_im, &upper_left_x, &upper_left_y, &last_scale);
    }
    
    cv::cvtColor(tmp_im, tmp_im, CV_BGR2BGRA);
    
    XImage *x_im = XCreateImage(dis, CopyFromParent, depth,
                                ZPixmap, 0, (char*)tmp_im.data,
                                tmp_im.cols, tmp_im.rows, 32, 0);

    Pixmap pix = XCreatePixmap(dis, win, tmp_im.cols, tmp_im.rows, depth);
    
    XPutImage(dis, pix, gc, x_im, 0, 0, 0, 0, tmp_im.cols, tmp_im.rows);

    XClearWindow(dis, win);

    
    
    XCopyArea(dis, pix, win, gc, 0, 0, tmp_im.cols, tmp_im.rows,
              upper_left_x, upper_left_y);

    XFlush(dis);
    
}

void WindowManager::generateImageToDrawFitToWindow(const cv::Mat& in_im, cv::Mat * const out_im,
                                                   int * const upper_left_x,
                                                   int * const upper_left_y,
                                                   double * const scale)const{

    const double tmp_scale = calcScaleToFitToWindow(in_im);
    
    cv::resize(in_im, *out_im, cv::Size(0, 0), tmp_scale, tmp_scale, cv::INTER_AREA);

    int width, height;

    getWindowSize(&width, &height);

    *upper_left_x = (width  - out_im->cols) / 2;
    *upper_left_y = (height - out_im->rows) / 2;

    if(scale){
        *scale = tmp_scale;
    }

    return;
}

void WindowManager::generateImageToDraw(const cv::Mat& in_im, cv::Mat * const out_im,
                                        int * const upper_left_x,
                                        int * const upper_left_y,
                                        double * const scale)const{

    const double tmp_scale = initial_scale * std::pow(scale_base, cur_scale_exponent);

    int u_l_x, u_l_y, l_r_x, l_r_y;
    getRegionToDraw(in_im, tmp_scale, &u_l_x, &u_l_y, &l_r_x, &l_r_y);

    u_l_x = u_l_x < 0 ? 0 : u_l_x;
    u_l_y = u_l_y < 0 ? 0 : u_l_y;
    l_r_x = l_r_x >= in_im.cols ? in_im.cols : l_r_x;
    l_r_y = l_r_y >= in_im.rows ? in_im.rows : l_r_y;

    cv::Rect roi(u_l_x, u_l_y, l_r_x - u_l_x, l_r_y - u_l_y);
                 
    // cv::resize(in_im(roi), out_im, )
    
    
    if(scale){
        *scale = tmp_scale;
    }
}

double WindowManager::calcScaleToFitToWindow(const cv::Mat& im)const{

    XWindowAttributes window_attributes;
    XGetWindowAttributes(dis, win, &window_attributes);

    const double scale_v = (double)window_attributes.height / im.rows;
    const double scale_h = (double)window_attributes.width / im.cols;
    double tmp_scale = 1.0;

    // calc min(1.0, scale_v, scale_h)
    if(scale_v < tmp_scale){
        tmp_scale = scale_v;
    }
    if(scale_h < tmp_scale){
        tmp_scale = scale_h;
    }

    return tmp_scale;
}

void WindowManager::getWindowSize(int * const width, int * const height)const{

    XWindowAttributes window_attributes;
    XGetWindowAttributes(dis, win, &window_attributes);

    *height = window_attributes.height;
    *width = window_attributes.width;

    return;
}

void WindowManager::setDefaultBackground(){

    const int depth = DefaultDepth(dis, screen);

    int width, height;
    {
        XWindowAttributes window_attributes;
        XGetWindowAttributes(dis, win, &window_attributes);
        width = window_attributes.width;
        height = window_attributes.height;
    }

    const int width_big = width % tile_size ? (width / tile_size + 1) * tile_size : width;
    const int height_big = height % tile_size ? (height / tile_size + 1) * tile_size : height;

    std::vector<XRectangle> rectangles;
    
    for(int i_v = 0; i_v < height_big / tile_size; i_v++){
        int count = i_v % 2;
        for(int i_h = 0; i_h < width_big / tile_size; i_h++){
            if(count % 2){
                XRectangle tmp_rect;
                tmp_rect.x = i_h * tile_size;
                tmp_rect.y = i_v * tile_size;
                tmp_rect.width  = tile_size;
                tmp_rect.height = tile_size;
                rectangles.push_back(tmp_rect);
            }
            count++;
        }
    }


    XColor gray1, gray2;
    gray1.red = gray1.green = gray1.blue = 0x4f00;
    gray2.red = gray2.green = gray2.blue = 0x4000;
    gray1.flags = gray2.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(dis, XDefaultColormap(dis, screen), &gray1);
    XAllocColor(dis, XDefaultColormap(dis, screen), &gray2);
    
    Pixmap pix = XCreatePixmap(dis, win, width, height, depth);

    XSetForeground(dis, gc, gray1.pixel);
    XFillRectangle(dis, pix, gc, 0, 0, width, height);

    XSetForeground(dis, gc, gray2.pixel);
    XFillRectangles(dis, pix, gc, rectangles.data(), (int)rectangles.size());

    XSetWindowBackgroundPixmap(dis, win, pix);    
}

void WindowManager::getRegionToDraw(const cv::Mat& in_im, const double scale,
                                    int * const upper_left_x, int * const upper_left_y,
                                    int * const lower_right_x, int * const lower_right_y)const{

    int window_width, window_height;

    getWindowSize(&window_width, &window_height);

    const int tmp_width = window_width / scale;
    const int tmp_hegiht = window_height / scale;

    upper_left_x = int(- (tmp_width / 2) / scale + in_im.cols / 2 + cur_offset_x);
    upper_left_y = int(- (tmp_height / 2) / scale + in_im.rows / 2 + cur_offset_y);
    lower_left_x = int(  (tmp_width / 2) / scale + in_im.cols / 2 + cur_offset_x);
    lower_left_y = int(  (tmp_height / 2) / scale + in_im.rows / 2 + cur_offset_y);

    return;
}


