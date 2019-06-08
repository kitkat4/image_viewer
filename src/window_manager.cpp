#include "window_manager.hpp"

WindowManager::WindowManager(){}

WindowManager::WindowManager(const std::string& window_name, const int width, const int height)
    :window_name(window_name){


    dis = XOpenDisplay(nullptr);

    if(! dis){
        std::cerr << my_utils_kk4::red
                  << "[ERROR] Failed to open a new window (" << __FILE__
                  << ", line " << __LINE__ << ")"
                  << my_utils_kk4::default_color
                  << std::endl;
    }
    
    screen = DefaultScreen(dis);

    unsigned long black = BlackPixel(dis, screen);
    unsigned long white = WhitePixel(dis, screen);

    win = XCreateSimpleWindow(dis, RootWindow(dis, screen), 0, 0, width, height,
                              5, white, black);

    XSetStandardProperties(dis, win, "Hoge", "Fuga", None, NULL, 0, NULL);

    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask | StructureNotifyMask);

    gc = XCreateGC(dis, win, 0, 0);

    XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);

    XClearWindow(dis, win);
    XMapRaised(dis, win);

    XFlush(dis);

    sleep(1);

}

WindowManager::~WindowManager(){}

void WindowManager::update(const cv::Mat& im){

    std::cerr << "[DEBUG] Called " << __func__ << std::endl;

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
        
        // KeySym key_sym = XKeycodeToKeysym(dis, x_event.xkey.keycode, 0);
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
        default:
            std::cerr << my_utils_kk4::yellow
                      << "[WARN ] Undefined command: " << key_sym 
                      << my_utils_kk4::default_color << std::endl;
            return NOTHING;
        }
        
    }else if(x_event.type == ConfigureNotify){
        std::cerr << "[DEBUG] Redrawing ... " << std::endl;
        return REDRAW;
    }else{
        std::cerr << my_utils_kk4::yellow
                  << "[WARN ] Undefined event: " << x_event.type << " at line " << __LINE__
                  << " in " << __FILE__  << my_utils_kk4::default_color << std::endl;
        return NOTHING;
    }
    
}

bool WindowManager::isShutdown()const{

    // TODO! 
    // check whether the window is closed
    return false;
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
    
    XWindowAttributes window_attributes;
    XGetWindowAttributes(dis, win, &window_attributes);

    const double scale_v = (double)window_attributes.height / im.rows;
    const double scale_h = (double)window_attributes.width / im.cols;
    double scale = 1.0;

    // calc min(1.0, scale_v, scale_h)
    if(scale_v < scale){
        scale = scale_v;
    }
    if(scale_h < scale){
        scale = scale_h;
    }

    cv::Mat tmp_im;
    cv::resize(im, tmp_im, cv::Size(0, 0), scale, scale);
    
    cv::cvtColor(tmp_im, tmp_im, CV_BGR2BGRA);
    
    XImage *x_im = XCreateImage(dis, CopyFromParent, depth,
                                ZPixmap, 0, (char*)tmp_im.data,
                                tmp_im.cols, tmp_im.rows, 32, 0);

    Pixmap pix = XCreatePixmap(dis, win, tmp_im.cols, tmp_im.rows, depth);
    
    XPutImage(dis, pix, gc, x_im, 0, 0, 0, 0, tmp_im.cols, tmp_im.rows);

    XClearWindow(dis, win);

    const int upper_left_x = (window_attributes.width  - tmp_im.cols) / 2;
    const int upper_left_y = (window_attributes.height - tmp_im.rows) / 2;
    
    XCopyArea(dis, pix, win, gc, 0, 0, tmp_im.cols, tmp_im.rows,
              upper_left_x, upper_left_y);

    XFlush(dis);
    
}

