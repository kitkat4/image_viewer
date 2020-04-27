#include "image_viewer.hpp"


ImageViewer::ImageViewer(const std::string& path){

    cur_im = cv::imread(path);
    cur_path = path;
    wm.reset(new WindowManager(cur_im.cols, cur_im.rows));
    wm->update(cur_im, cur_path);
    
    dir_scanner.reset(new DirScanner(path));
}

ImageViewer::~ImageViewer(){

}

void ImageViewer::enterMainLoop(){

    if(! dir_scanner){
        std::cerr << my_utils_kk4::red
                  << "[ERROR] No instance created"
                  << my_utils_kk4::default_color
                  << std::endl;
        return;
    }
    
    while(true){

        Command c = wm->nextCommand();

        if(c == Command::REDRAW){
            
            update();
            
        }else if(c == Command::NEXT_IM){

            dir_scanner->goToNextIm();
            update();
            
        }else if(c == Command::PREVIOUS_IM){

            dir_scanner->goToPreviousIm();
            update();

        }else if(c == Command::UPPER_DIR){

            dir_scanner->goToParentDir();
            std::cerr << "[INFO ] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();
                
        }else if(c == Command::LOWER_DIR){

            dir_scanner->goToChildDirUsingHistory();
            std::cerr << "[INFO ] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();            
            
        }else if(c == Command::NEXT_DIR){

            dir_scanner->goToNextBrotherDir();
            std::cerr << "[INFO ] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();
            
        }else if(c == Command::PREVIOUS_DIR){

            dir_scanner->goToPreviousBrotherDir();
            std::cerr << "[INFO ] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();

        }else if(c == Command::SCALE_UP){

            std::cerr << "[INFO ] Scaling up" << std::endl;
            wm->scaleUp();
            update();
            
        }else if(c == Command::SCALE_DOWN){

            std::cerr << "[INFO ] Scaling down" << std::endl;
            wm->scaleDown();
            update();

        }else if(c == Command::MOVE_RIGHT){

            std::cerr << "[INFO ] Moving right" << std::endl;
            wm->moveRight();
            update();

        }else if(c == Command::MOVE_LEFT){

            std::cerr << "[INFO ] Moving left" << std::endl;
            wm->moveLeft();
            update();
            
        }else if(c == Command::MOVE_UP){

            std::cerr << "[INFO ] Moving up" << std::endl;
            wm->moveUp();
            update();
            
        }else if(c == Command::MOVE_DOWN){

            std::cerr << "[INFO ] Moving down" << std::endl;
            wm->moveDown();
            update();

        }else if(c == Command::CLEAR){

            std::cerr << "[IFNO ] Clear scale factor and offset" << std::endl;
            wm->clearScaleAndOffset();
            update();
            
        }else if(c == Command::QUIT){
            
            wm->closeWindow();
            return;
        }
        
    } // end of while
}

void ImageViewer::update(){

    if(dir_scanner){

        if(*dir_scanner){
            std::string tmp_path = dir_scanner->getCurrentIm();

            if(tmp_path != cur_path){

                cur_path = tmp_path;
                cur_im = cv::imread(cur_path);
            }
        }else{
            cur_path = dir_scanner->getCurrentDir();
            cur_im = cv::Mat();
        }
    }
    
    wm->update(cur_im, cur_path);
}


