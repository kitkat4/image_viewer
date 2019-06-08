#include "image_viewer.hpp"



ImageViewer::ImageViewer(const std::string& path, const std::string& window_name)
    :window_name(window_name){

    cur_im_original = cv::imread(path);
    cur_im = cur_im_original.clone();
    cur_path = path;
    wm.reset(new WindowManager(window_name, cur_im.cols, cur_im.rows));
    wm->update(cur_im);
    
    cur_instance = new DirScanner(path);

}

ImageViewer::~ImageViewer(){

}

void ImageViewer::enterMainLoop(){

    std::cerr << "[DEBUG] Called " << __func__ << std::endl;
    
    if(! cur_instance){
        std::cerr << my_utils_kk4::red
                  << "[ERROR] No instance created"
                  << my_utils_kk4::default_color
                  << std::endl;
        return;
    }
    
    DirScanner* new_instance;
    
    while(true){

        Command c = wm->nextCommand();

        if(c == Command::REDRAW){
            
            update();
            
        }else if(c == Command::NEXT_IM){
            
            cur_instance->goToNextIm();

            update();
            
        }else if(c == Command::PREVIOUS_IM){
            
            cur_instance->goToPreviousIm();

            update();

        }else if(c == Command::UPPER_DIR){
            
            cur_instance->goToParentDir();
            
            std::cerr << "[INFO ] Moving to " << cur_instance->getCurrentPath() << std::endl;

            update();
                
        }else if(c == Command::LOWER_DIR){

            cur_instance->goToChildDir();
            
            std::cerr << "[INFO ] Moving to " << cur_instance->getCurrentPath() << std::endl;

            update();            
            
        }else if(c == Command::NEXT_DIR){

            cur_instance->goToNextBrotherDir();
            
            std::cerr << "[INFO ] Moving to " << cur_instance->getCurrentPath() << std::endl;

            update();
            
        }else if(c == Command::PREVIOUS_DIR){

            cur_instance->goToPreviousBrotherDir();
            
            std::cerr << "[INFO ] Moving to " << cur_instance->getCurrentPath() << std::endl;

            update();
        }else if(c == Command::QUIT){
            wm->closeWindow();
            return;
        }

        if(wm->isShutdown()){
            wm->closeWindow();
            return;
        }
    } // end of while
}

void ImageViewer::update(){

    if(cur_instance){

        if(*cur_instance){
            std::string tmp_path = cur_instance->getCurrentIm();

            if(tmp_path != cur_path){

                cur_path = tmp_path;
                cur_im_original = cv::imread(cur_path);
                cur_im = cur_im_original.clone();
            }
        }else{
            cur_path = "";
            cur_im_original = cv::Mat();
            cur_im = cv::Mat();
        }
    }
    
    wm->update(cur_im);
}


