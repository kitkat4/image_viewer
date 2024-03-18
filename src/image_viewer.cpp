#include "image_viewer.hpp"


ImageViewer::ImageViewer(const std::string& path){

    std::cout << "Help" << std::endl
              << std::setw(18) << "Right" << ": " << "NEXT_IM" << std::endl
              << std::setw(18) << "Shift + Right" << ": " << "MOVE_RIGHT" << std::endl
              << std::setw(18) << "Left" << ": " << "PREVIOUS_IM" << std::endl
              << std::setw(18) << "Shift + Left" << ": " << "MOVE_LEFT" << std::endl
              << std::setw(18) << "Return" << ": " << "NEXT_IM" << std::endl
              << std::setw(18) << "Shift + Return" << ": " << "PREVIOUS_IM" << std::endl
              << std::setw(18) << "Up" << ": " << "PREVIOUS_BROTHER_DIR" << std::endl
              << std::setw(18) << "Shift + Up" << ": " << "MOVE_UP" << std::endl
              << std::setw(18) << "Ctrl + Up" << ": " << "SCALE_UP" << std::endl
              << std::setw(18) << "Down" << ": " << "NEXT_BROTHER_DIR" << std::endl
              << std::setw(18) << "Shift + Down" << ": " << "MOVE_DOWN" << std::endl
              << std::setw(18) << "Ctrl + Down" << ": " << "SCALE_DOWN" << std::endl
              << std::setw(18) << "Page_Up" << ": " << "UPPER_DIR" << std::endl            
              << std::setw(18) << "Page_Down" << ": " << "LOWER_DIR" << std::endl
              << std::setw(18) << "+" << ": " << "SCALE_UP" << std::endl
              << std::setw(18) << "-" << ": " << "SCALE_DOWN" << std::endl
              << std::setw(18) << "c" << ": " << "CLEAR" << std::endl
              << std::setw(18) << "q" << ": " << "QUIT" << std::endl
              << std::setw(18) << "Left click" << ": " << "Keep pixel info" << std::endl
              << std::setw(18) << "Wheel up" << ": " << "PREVIOUS_IM" << std::endl
              << std::setw(18) << "Ctrl + Wheel up" << ": " << "SCALE_UP" << std::endl
              << std::setw(18) << "Wheel down" << ": " << "NEXT_IM" << std::endl
              << std::setw(18) << "Ctrl + Wheel down" << ": " << "SCALE_DOWN" << std::endl
              << std::setw(18) << "Drag" << ": " << "Move image" << std::endl
              << std::endl;
        
    dir_scanner.reset(new DirScanner(path));
    std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
    update();
    wm.reset(new WindowManager(cur_im.cols, cur_im.rows));
    update();
}

ImageViewer::~ImageViewer(){

    std::cout << std::endl;
}

void ImageViewer::enterMainLoop(){

    if(! dir_scanner){
        std::cerr << "[ERROR] No instance created"
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
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();
                
        }else if(c == Command::LOWER_DIR){

            dir_scanner->goToChildDirUsingHistory();
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();            
            
        }else if(c == Command::NEXT_DIR){

            dir_scanner->goToNextImDir();
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();

        }else if(c == Command::NEXT_BROTHER_DIR){

            dir_scanner->goToNextBrotherDir();
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();
            
        }else if(c == Command::PREVIOUS_DIR){

            dir_scanner->goToPreviousImDir();
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();

        }else if(c == Command::PREVIOUS_BROTHER_DIR){

            dir_scanner->goToPreviousBrotherDir();
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();

        }else if(c == Command::NEXT_PARENT){

            dir_scanner->goToFirstImDirUnderNextParentDir();
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();
            
        }else if(c == Command::PREVIOUS_PARENT){

            dir_scanner->goToLastImDirUnderPreviousParentDir();
            std::cerr << "[ INFO] Moving to " << dir_scanner->getCurrentDir() << std::endl;
            update();

        }else if(c == Command::SCALE_UP){

            wm->scaleUp();
            update();
            
        }else if(c == Command::SCALE_DOWN){

            wm->scaleDown();
            update();

        }else if(c == Command::MOVE_RIGHT){

            wm->moveRight();
            update();

        }else if(c == Command::MOVE_LEFT){

            wm->moveLeft();
            update();
            
        }else if(c == Command::MOVE_UP){

            wm->moveUp();
            update();
            
        }else if(c == Command::MOVE_DOWN){

            wm->moveDown();
            update();

        }else if(c == Command::CLEAR){

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
                std::cout << "\r                                                                  "
                          << std::endl
                          << cur_path << std::endl
                          << "width   : " << cur_im.cols << std::endl
                          << "height  : " << cur_im.rows << std::endl
                          << "channels: " << cur_im.channels() << std::endl;
                const int channels = cur_im.channels();
                if(cur_im.type() == CV_8UC(channels)){
                    std::cout << "type    : 8U" << std::endl;
                }else if(cur_im.type() == CV_8SC(channels)){
                    std::cout << "type    : 8S" << std::endl;
                }else if(cur_im.type() == CV_16UC(channels)){
                    std::cout << "type    : 16U" << std::endl;
                }else if(cur_im.type() == CV_16SC(channels)){
                    std::cout << "type    : 16S" << std::endl;
                }else if(cur_im.type() == CV_32SC(channels)){
                    std::cout << "type    : 32S" << std::endl;
                }else if(cur_im.type() == CV_32FC(channels)){
                    std::cout << "type    : 32F" << std::endl;
                }else if(cur_im.type() == CV_64FC(channels)){
                    std::cout << "type    : 64F" << std::endl;
                }else{
                    std::cout << "type    : unknown" << std::endl;
                }
            }

        }else{
            cur_path = dir_scanner->getCurrentDir();
            cur_im = cv::Mat();
        }
    }

    if(wm){
        wm->update(cur_im, cur_path);
    }
}



