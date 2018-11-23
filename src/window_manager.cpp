#include "window_manager.hpp"

WindowManager::WindowManager(){}

WindowManager::WindowManager(const std::string& window_name)
    :window_name(window_name){

#ifdef IMAGE_VIEWER_WITH_OPENCV_WINDOW
    
    // Open a window
    cv::namedWindow(window_name, CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    
#elif defined IMAGE_VIEWER_WITH_X11_WINDOW

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

    win = XCreateSimpleWindow(dis, RootWindow(dis, screen), 0, 0, 1900, 1000, 5, white, black);

    XSetStandardProperties(dis, win, "Hoge", "Fuga", None, NULL, 0, NULL);

    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);

    gc = XCreateGC(dis, win, 0, 0);

    XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);

    XClearWindow(dis, win);
    XMapRaised(dis, win);

    XFlush(dis);

    sleep(1);
    
#else

    std::cerr << "[ERROR] oops!" << std::endl;
    
#endif

}

WindowManager::~WindowManager(){}

void WindowManager::update(const cv::Mat& im, const std::string& current_path){

    std::cerr << "[DEBUG] Called " << __func__ << std::endl;

#ifdef IMAGE_VIEWER_WITH_OPENCV_WINDOW

    cv::imshow(window_name, im);
    cv::displayStatusBar(window_name, current_path, 1000);
    
#elif defined IMAGE_VIEWER_WITH_X11_WINDOW
    
    drawImage(im);
    
#endif
    
}

WindowManager::Command WindowManager::nextCommand()const{

#ifdef IMAGE_VIEWER_WITH_OPENCV_WINDOW

    char c = cv::waitKey(10);

    switch(c){
    case 'S':                   // right
        return NEXT_IM;
    case 'Q':                   // left
        return PREVIOUS_IM;
    case 'R':                   // up
        return PREVIOUS_DIR;
    case 'T':                   // down
        return NEXT_DIR;
    case 'U':                   // PgUp
        return UPPER_DIR;
    case 'V':                   // PgDn
        return LOWER_DIR;
    case -1:
        return NOTHING;
    default:
        std::cerr << my_utils_kk4::yellow
                  << "[WARNING] " << c << " is an undefined command"
                  << my_utils_kk4::default_color << std::endl;
        return NOTHING;
    }
    
#elif defined IMAGE_VIEWER_WITH_X11_WINDOW
    

    // do something
    
#endif
    
    
}

bool WindowManager::isShutdown()const{

#ifdef IMAGE_VIEWER_WITH_OPENCV_WINDOW
    
    try{
        if(cv::getWindowProperty(window_name, CV_WND_PROP_VISIBLE) == 0){
            // if(! cv::GetWindowHandle(window_name.c_str())){
            return true; // when the window is closed with a mouse click or Alt+F4
        }
    }catch(std::exception){
        return true;
    }

    return false;

#elif defined IMAGE_VIEWER_WITH_X11_WINDOW
    
    // do something
    return false;
    
#endif

}


void WindowManager::closeWindow(){

#ifdef IMAGE_VIEWER_WITH_OPENCV_WINDOW
    cv::destroyWindow(window_name);
#elif defined IMAGE_VIEWER_WITH_X11_WINDOW

    XFreeGC(dis, gc);
	XDestroyWindow(dis,win);
	XCloseDisplay(dis);	

#endif
    
}


#ifdef IMAGE_VIEWER_WITH_X11_WINDOW

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
    
    XCopyArea(dis, pix, win, gc, 0, 0, 1900, 1000, 0, 0);

    XFlush(dis);
    
}

#endif
