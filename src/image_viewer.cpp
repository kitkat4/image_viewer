#include "image_viewer.hpp"



ImageViewer::ImageViewer(const std::string& path, const std::string& window_name)
    :window_name(window_name){

    cur_instance = new DirScanner(path, window_name, this);
}

ImageViewer::~ImageViewer(){
    
}

void ImageViewer::enterMainLoop(){

    // Open a window
    if(! window_opened){
        cv::namedWindow(window_name, CV_WINDOW_NORMAL | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
        window_opened = true;
    }

    if(! cur_instance){
        std::cerr << "[ERROR] No instance created" << std::endl;
        return;
    }
    
    DirScanner* new_instance;
    
    while(true){
        new_instance = cur_instance->mainLoop();
        if(new_instance != cur_instance){
            delete cur_instance;
            cur_instance = new_instance;
        }
        
        if(! new_instance){
            cv::destroyWindow(window_name);
            return;
        }
    }
}

