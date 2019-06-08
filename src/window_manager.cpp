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

void WindowManager::update(const cv::Mat& im, const std::string& current_path){

    std::cerr << "[DEBUG] Called " << __func__ << std::endl;

    drawImage(im);
    
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

    std::cerr << "[DEBUG] Called " << __func__ << std::endl;

    std::cerr << "[DEBUG] Func " << __func__ << " at line " << __LINE__ << " of file " << __FILE__ << std::endl;

    XColor c;
    cv::Vec3b v;
    Colormap cmap = XDefaultColormap(dis, screen);

    

    std::cerr << "[DEBUG] Func " << __func__ << " at line " << __LINE__ << " of file " << __FILE__ << std::endl;


    std::cerr << "[DEBUG] Default depth: " << DefaultDepth(dis, screen) << std::endl;
    const int depth = DefaultDepth(dis, screen);

    cv::Mat tmp_im;
    cv::cvtColor(im, tmp_im, CV_BGR2BGRA);
    XImage *x_im = XCreateImage(dis, CopyFromParent, depth,
                                ZPixmap, 0, (char*)tmp_im.data,
                                im.cols, im.rows, 32, 0);


    std::cerr << "[DEBUG] Func " << __func__ << " at line " << __LINE__ << " of file " << __FILE__ << std::endl;

    Pixmap pix = XCreatePixmap(dis, win, im.cols, im.rows, depth);

    std::cerr << "[DEBUG] Func " << __func__ << " at line " << __LINE__ << " of file " << __FILE__ << std::endl;
    
    XPutImage(dis, pix, gc, x_im, 0, 0, 0, 0, im.cols, im.rows);

    std::cerr << "[DEBUG] Func " << __func__ << " at line " << __LINE__ << " of file " << __FILE__ << std::endl;

    std::cerr << "[DEBUG] Func " << __func__ << " at line " << __LINE__ << " of file " << __FILE__ << std::endl;

    XClearWindow(dis, win);
    
    XCopyArea(dis, pix, win, gc, 0, 0, im.cols, im.rows, 0, 0);

    XFlush(dis);
    
}

