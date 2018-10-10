#include "image_viewer.hpp"


ImageViewer::ImageViewer(const std::string& path)
    :ok(true), cur_im(cv::Mat()), cur_dir(fs::path()),
     im_ix(0){


    if(fs::is_regular_file(path)){ // image path

        cur_im = cv::imread(path);

        if(cur_im.empty()){
            std::cerr << "[ERROR] Failed to open " << path << std::endl;
            ok = false;
        }

        fs::path im_path(path);

        cur_dir = im_path.parent_path();

        entries.clear();
        int i = 0;
        for(auto& itr : fs::directory_iterator(cur_dir)){
            fs::path tmp = itr.path();
            if(tmp.compare(im_path) == 0){
                im_ix = i;
            }
            entries.push_back(tmp.c_str());
            i++;
        }

            
    }else{                      // directory path
        
        cur_dir.assign(path);

        im_ix = -1;

        entries.clear();
        int i = 0;
        for(auto& itr : fs::directory_iterator(cur_dir)){
            fs::path tmp = itr.path();
            if(im_ix == -1){
                cur_im = cv::imread(tmp.c_str());
                if(! cur_im.empty()){
                    im_ix = i;
                }
            }
            entries.push_back(tmp.c_str());
            i++;
        }

        if(im_ix == -1){
            std::cerr << "[ERROR] No valid image found" << std::endl;
            ok = false;
        }

    }

    // Open a window
    if(! window_opened){
        cv::namedWindow(window_name);
        window_opened = true;
    }

    if(ok && ! cur_instance)
        cur_instance = this;
        
}


ImageViewer::~ImageViewer(){

}

ImageViewer* ImageViewer::mainLoop(){

    cv::imshow(window_name, cur_im);

    while(true){
        
        char c = cv::waitKey(10);

        if(c == 'S' || c == 'Q'){
            
            if(c == 'S'){
                std::cout << "next image" << std::endl;
                im_ix++;
            }else if(c == 'Q'){
                std::cout << "previous image" << std::endl;
                im_ix--;
            }
            
            cur_im = cv::imread(entries[im_ix]);
            
            if(cur_im.empty()){
                std::cerr << "[ERROR] Failed to open " << entries[im_ix] << std::endl;
                ok = false;
                cv::destroyWindow(window_name);
                return nullptr;
            }else{
                cv::imshow(window_name, cur_im);
                continue;
            }
        }else if(c != -1){
            std::cout << c << std::endl;
        }

        try{
            if(cv::getWindowProperty(window_name, 0) < 0){
                return nullptr; // when the window is closed with a mouse click or Alt+F4
            }
        }catch(std::exception){
            return nullptr;
        }
    }
}

void ImageViewer::enterMainLoop(){

    ImageViewer* instance = cur_instance;
    
    while(true){
        ImageViewer* instance = instance->mainLoop();
        
        if(! instance){
            return;
        }
    }
}

const std::string ImageViewer::window_name = "Image Viewer";
bool ImageViewer::window_opened = false;
ImageViewer* ImageViewer::cur_instance = nullptr;

