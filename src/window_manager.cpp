#include "window_manager.hpp"

WindowManager::WindowManager(){}

WindowManager::WindowManager(const std::string& window_name)
    :window_name(window_name){

#ifdef IMAGE_VIEWER_WITH_OPENCV_WINDOW
    
    // Open a window
    cv::namedWindow(window_name, CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
    
#elif defined IMAGE_VIEWER_WITH_X11_WINDOW

    // do something
    
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

