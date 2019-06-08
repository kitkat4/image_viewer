#include "dir_scanner.hpp"



DirScanner::DirScanner(const std::string& path, const std::string& window_name,
                       ImageViewer* const viewer, WindowManager* const wm)
    :ok(true), cur_dir(fs::path()),
     im_ix(0), window_name(window_name), viewer(viewer), wm(wm){

    if(fs::is_regular_file(path)){ // image path

        viewer->cur_im_original = cv::imread(path);
        viewer->cur_im = viewer->cur_im_original.clone();
        viewer->cur_path = fs::path(path).generic_string();


        if(viewer->cur_im.empty()){
            std::cerr << my_utils_kk4::red
                      << "[ERROR] Failed to open " << path
                      << my_utils_kk4::default_color
                      << std::endl;
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
                if(fs::is_regular_file(tmp) && isImageFile(tmp.c_str())){
                    viewer->cur_im_original = cv::imread(tmp.c_str());
                    viewer->cur_im = viewer->cur_im_original.clone();
                    viewer->cur_path = tmp.generic_string();
                    if(! viewer->cur_im.empty()){
                        im_ix = i;
                    }
                }
            }
            entries.push_back(tmp.c_str());
            i++;
        }

        if(im_ix == -1){
            std::cerr << my_utils_kk4::yellow
                      << "[WARN ] No valid image found"
                      << my_utils_kk4::default_style << std::endl;
            // cv::cvtColor(viewer->cur_im_original, viewer->cur_im, CV_RGB2GRAY);
            viewer->cur_im = cv::Mat::zeros(viewer->cur_im_original.size(), CV_8UC4);
            viewer->cur_path = cur_dir.generic_string();
            // cv::putText(viewer->cur_im, path, cv::Point(200,200), cv::FONT_HERSHEY_SIMPLEX, 4.0,
            //             cv::Scalar(255,255,255), 4, 8, false);
            ok = false;
        }

    }

}


DirScanner::~DirScanner(){
}

DirScanner* DirScanner::mainLoop(){

    // cv::setWindowProperty(window_name, CV_WND_PROP_ASPECTRATIO, CV_WINDOW_KEEPRATIO);

    std::cerr << "[DEBUG] Called " << __func__ << std::endl;


    wm->update(viewer->cur_im, viewer->cur_path);

    
    while(true){

        Command c = wm->nextCommand();

        if(c == Command::NEXT_IM || c == Command::PREVIOUS_IM){ 

            int check_count = 0;
                
            while(true){

                check_count++;

                if(check_count > (int)entries.size()){
                    break;
                }
                
                if(c == Command::NEXT_IM){
                    im_ix++;
                }else if(c == Command::PREVIOUS_IM){
                    im_ix--;
                }
            
                if(im_ix >= (int)entries.size()){
                    im_ix = 0;
                }else if(im_ix < 0){
                    im_ix = entries.size() - 1;
                }

                if((! fs::is_regular_file(entries[im_ix])) || (! isImageFile(entries[im_ix]))){
                    continue;
                }

                viewer->cur_im_original = cv::imread(entries[im_ix]);
                viewer->cur_im = viewer->cur_im_original.clone();
                viewer->cur_path = entries[im_ix];
            

                if(! viewer->cur_im.empty()){
                    break;
                }

            }

            wm->update(viewer->cur_im, viewer->cur_path);
            
                
            continue;

        }else if(c == Command::UPPER_DIR){
            fs::path p = cur_dir.parent_path();
            if(p.generic_string().size() > 0){
                std::cerr << "[INFO ] Moving to " << cur_dir.parent_path() << std::endl;
                return new DirScanner(cur_dir.parent_path(), window_name, viewer, wm);
            }else{
                std::cerr << my_utils_kk4::red
                          << "[ERROR] Unexpected error at line " << __LINE__ 
                          << " in " << __FILE__
                          << my_utils_kk4::default_style << std::endl;
            }
        }else if(c == Command::LOWER_DIR){
            for(auto& itr : entries){
                if(! fs::is_regular_file(itr)){
                    std::cerr << "[INFO ] Moving to " << itr << std::endl;
                    return new DirScanner(itr, window_name, viewer, wm);
                }
            }
        }else if(c == Command::NEXT_DIR){
            std::cerr << "[INFO ] Moving to " << nextBrotherDir() << std::endl;
            return new DirScanner(nextBrotherDir(), window_name, viewer, wm);
        }else if(c == Command::PREVIOUS_DIR){
            std::cerr << "[INFO ] Moving to " << previousBrotherDir() << std::endl;
            return new DirScanner(previousBrotherDir(), window_name, viewer, wm);
        }

        if(wm->isShutdown()){
            return nullptr;
        }
        
    }
}

std::string DirScanner::nextBrotherDir()const{

    fs::path p = cur_dir.parent_path();

    if(p.generic_string().size() == 0){
        return cur_dir.generic_string();
    }

    bool finish = false;
    std::string first_found_path;
    for(auto& itr: fs::directory_iterator(p)){
        if(first_found_path.empty() && ! fs::is_regular_file(itr)){
            first_found_path = itr.path().generic_string();
        }
        if(finish){
            if(! fs::is_regular_file(itr)){
                return itr.path().generic_string();
            }
        }else{
            if(itr.path() == cur_dir){
                finish = true;
            }
        }
    }

    if(finish){
        return first_found_path;
    }else{
        return cur_dir.generic_string();
    }
}

std::string DirScanner::previousBrotherDir()const{

    fs::path p = cur_dir.parent_path();

    if(p.generic_string().size() == 0){
        return cur_dir.generic_string();
    }

    fs::path tmp_path = cur_dir;
    for(auto& itr: fs::directory_iterator(p)){
        if(! fs::is_regular_file(itr.path())){
            tmp_path = itr.path();
        }
    }

    fs::path prev_path = cur_dir;
    for(auto& itr: fs::directory_iterator(p)){
        if(! fs::is_regular_file(tmp_path)){
            prev_path = tmp_path;
        }
        tmp_path = itr.path();
        if(tmp_path == cur_dir){
            return prev_path.generic_string();
        }
    }

    return cur_dir.generic_string();
}


bool DirScanner::isImageFile(const std::string& path_str){

    fs::path fspath(path_str);

    if(! fs::is_regular_file(fspath)){
        return false;
    }

    std::string ext = fspath.extension();

    return ext == ".bmp"
        || ext == ".dib"
        || ext == ".jpeg"
        || ext == ".jpg"
        || ext == ".jpe"
        || ext == ".jp2"
        || ext == ".png"
        || ext == ".pbm"
        || ext == ".pgm"
        || ext == ".ppm"
        || ext == ".sr"
        || ext == ".ras"
        || ext == ".tiff"
        || ext == ".tif"
        || ext == ".BMP"
        || ext == ".DIB"
        || ext == ".JPEG"
        || ext == ".JPG"
        || ext == ".JPE"
        || ext == ".JP2"
        || ext == ".PNG"
        || ext == ".PBM"
        || ext == ".PGM"
        || ext == ".PPM"
        || ext == ".SR"
        || ext == ".RAS"
        || ext == ".TIFF"
        || ext == ".TIF";

}



