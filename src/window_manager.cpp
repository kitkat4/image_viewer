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

    // win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 200, 300, 5, white, black);
    win = XCreateSimpleWindow(dis, RootWindow(dis, screen), 0, 0, 200, 300, 1, white, black);

    XSetStandardProperties(dis, win, "Hoge", "Fuga", None, NULL, 0, NULL);

    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);

    gc = XCreateGC(dis, win, 0, 0);

    XSetBackground(dis,gc,white);
	XSetForeground(dis,gc,black);

    XClearWindow(dis, win);
    XMapRaised(dis, win);
    
    
#else

    std::cerr << "[ERROR] oops!" << std::endl;
    
#endif

}

WindowManager::~WindowManager(){}

void WindowManager::update(const cv::Mat& im, const std::string& current_path){

#ifdef IMAGE_VIEWER_WITH_OPENCV_WINDOW

    cv::imshow(window_name, im);
    cv::displayStatusBar(window_name, current_path, 1000);
    
#elif defined IMAGE_VIEWER_WITH_X11_WINDOW
    

    // do something
    
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
        return UPPER_DIR;
    case 'T':                   // down
        return LOWER_DIR;
    case 'U':                   // PgUp
        return NEXT_DIR;
    case 'V':                   // PgDn
        return PREVIOUS_DIR;
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

