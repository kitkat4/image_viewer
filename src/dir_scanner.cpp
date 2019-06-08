#include "dir_scanner.hpp"



DirScanner::DirScanner(const std::string& path)
    :ok(false), cur_dir(fs::path()), im_ix(-1){

    if(! fs::is_regular_file(fs::path(path))){ // directory given
        cur_dir.assign(path);
        scanEntries();
        findFirstIm();
    }else{                      // image given
        if(isImageFile(path)){
            cur_dir.assign(fs::path(path).parent_path());
            scanEntries();
            
            // check the im_ix of the given image
            for(int i = 0; i < (int)entries.size(); i++){
                if(fs::path(entries[i]).filename() == fs::path(path).filename()){
                    im_ix = i;
                    ok = true;
                    break;
                }
            }
        }else{
            std::cerr << my_utils_kk4::red
                      << "[ERROR] The given path is not directory nor image file"
                      << my_utils_kk4::default_style << std::endl;
        }
    }
}


DirScanner::~DirScanner(){
}

std::string DirScanner::getCurrentIm()const{

    if(entries.size() > 0){
        return entries[im_ix];
    }else{
        std::cerr << my_utils_kk4::red
                  << "[ERROR] No images found."
                  << my_utils_kk4::default_style << std::endl;
        return "";
    }
}

std::string DirScanner::goToNextIm(){

    int check_count = 0;

    const int old_im_ix = im_ix;
                
    while(true){

        // no image found
        if(check_count >= (int)entries.size()){
            std::cerr << my_utils_kk4::red
                      << "[ERROR] Unexpected error occured while searching the next image, "
                      << "at line " << __LINE__ << " in " << __FILE__
                      << my_utils_kk4::default_style << std::endl;
            ok = false;
            im_ix = -1;
            return "";
        }
                
        im_ix++;
            
        if(im_ix >= (int)entries.size()){
            im_ix = 0;
        }
        
        if(fs::is_regular_file(fs::path(entries[im_ix])) && isImageFile(entries[im_ix])){
            if(im_ix == old_im_ix){
                std::cerr << "[INFO ] Image search finished, but no new images found."
                          << std::endl;
            }
            ok = true;
            return entries[im_ix];
        }

        check_count++;
    }
    
}

std::string DirScanner::goToPreviousIm(){

    int check_count = 0;

    const int old_im_ix = im_ix;
                
    while(true){

        // no image found
        if(check_count >= (int)entries.size()){
            std::cerr << my_utils_kk4::red
                      << "[ERROR] Unexpected error occured while searching the next image, "
                      << "at line " << __LINE__ << " in " << __FILE__
                      << my_utils_kk4::default_style << std::endl;
            ok = false;
            im_ix = -1;
            return "";
        }
                
        im_ix--;
            
        if(im_ix < 0){
            im_ix = entries.size() - 1;
        }
        
        if(fs::is_regular_file(fs::path(entries[im_ix])) && isImageFile(entries[im_ix])){
            
            if(im_ix == old_im_ix){
                std::cerr << "[INFO ] Image search finished, but no new images found."
                          << std::endl;
            }
            ok = true;
            return entries[im_ix];
        }

        check_count++;
    }
}

void DirScanner::goToParentDir(){

    child_dir_history.push_back(cur_dir);
    cur_dir = cur_dir.parent_path();

    scanEntries();
    findFirstIm();
}

void DirScanner::goToChildDir(){

    if(! child_dir_history.empty()){

        cur_dir = child_dir_history.back();
        child_dir_history.pop_back();
        scanEntries();
        findFirstIm();
        
    }else{
        for(auto& itr : entries){
            if(! fs::is_regular_file(fs::path(itr))){
                
                cur_dir.assign(itr);
                scanEntries();
                findFirstIm();
                break;
            }
        }
    }
}

void DirScanner::goToNextBrotherDir(){

    child_dir_history.clear();

    cur_dir.assign(nextBrotherDir());

    scanEntries();
    findFirstIm();
}

void DirScanner::goToPreviousBrotherDir(){

    child_dir_history.clear();

    cur_dir.assign(previousBrotherDir());

    scanEntries();
    findFirstIm();
}


std::string DirScanner::getCurrentDir()const{

    return cur_dir.generic_string();
}

void DirScanner::scanEntries(){

    entries.clear();
    for(auto& itr : fs::directory_iterator(cur_dir)){
        entries.push_back(itr.path().c_str());
    }
}

void DirScanner::findFirstIm(){

    for(int i = 0; i < (int)entries.size(); i++){

        if(fs::is_regular_file(fs::path(entries[i])) && isImageFile(entries[i])){
            im_ix = i;
            ok = true;
            return;
        }
    }

    std::cerr << my_utils_kk4::yellow
              << "[WARN ] No valid image found"
              << my_utils_kk4::default_style << std::endl;
    ok = false;
    im_ix = -1;
}

std::string DirScanner::nextBrotherDir()const{

    fs::path p = cur_dir.parent_path();

    if(p.generic_string().size() == 0){
        return cur_dir.generic_string();
    }

    bool finish = false;
    std::string first_found_path;
    for(auto& itr: fs::directory_iterator(p)){
        if(first_found_path.empty() && ! fs::is_regular_file(itr.path())){
            first_found_path = itr.path().generic_string();
        }
        if(finish){
            if(! fs::is_regular_file(itr.path())){
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
        std::cerr << my_utils_kk4::red
                  << "[ERROR] Unexpected error occured at line " << __LINE__
                  << " in " << __FILE__ << my_utils_kk4::default_style << std::endl;
        return cur_dir.generic_string();
    }
}

std::string DirScanner::previousBrotherDir()const{

    fs::path p = cur_dir.parent_path();

    if(p.generic_string().size() == 0){
        return cur_dir.generic_string();
    }

    fs::path prev_path;

    // find last dir
    for(auto& itr: fs::directory_iterator(p)){
        if(! fs::is_regular_file(itr.path())){
            prev_path = itr.path();
        }
    }
    
    for(auto& itr: fs::directory_iterator(p)){
        
        if(itr.path() == cur_dir){
            return prev_path.generic_string();
        }else if(! fs::is_regular_file(itr.path())){
            prev_path = itr.path();
        }
    }

    std::cerr << my_utils_kk4::red
              << "[ERROR] Unexpected error occured at line " << __LINE__
              << " in " << __FILE__ << my_utils_kk4::default_style << std::endl;
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



