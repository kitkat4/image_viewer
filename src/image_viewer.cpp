#include "image_viewer.hpp"



ImageViewer::ImageViewer(const std::string& path, const std::string& window_name)
    :window_name(window_name), wm(new WindowManager(window_name)){

    cur_instance = new DirScanner(path, window_name, this, wm.get());
}

ImageViewer::~ImageViewer(){

}

void ImageViewer::enterMainLoop(){

    if(! window_opened){
        WindowManager wm(window_name);
        window_opened = true;
    }
    
    if(! cur_instance){
        std::cerr << my_utils_kk4::red
                  << "[ERROR] No instance created"
                  << my_utils_kk4::default_color
                  << std::endl;
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
            wm->closeWindow();
            return;
        }
    }
}

