#include "dir_scanner.hpp"



DirScanner::DirScanner(const std::string& path)
    :ok(false), cur_dir(fs::path()), loop_dir(false), im_ix(-1){

    if(! fs::is_regular_file(fs::path(path))){ // directory given

        cur_dir = fs::path(path);
        updateEntries();
        findFirstIm();

        if(! ok){
            goToNextImDir();
        }
        
    }else{                      // image given
        if(isImageFile(path)){
            cur_dir = fs::path(path).parent_path();
            updateEntries();
            
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
            std::cerr << "check_count: " << check_count << std::endl;
            std::cerr << "entries.size: " << entries.size() << std::endl;
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

bool DirScanner::goToNextDir(){

    bool success = false;

    success = goToFirstChildDir();

    if(! success){
        success = goToNextBrotherDir();
    }

    fs::path tmp_cur_dir = cur_dir;

    while(! success){
        if(! goToParentDir()){
            cur_dir = tmp_cur_dir;
            updateEntries();
            findFirstIm();
            return false;
        }
        success = goToNextBrotherDir();
    }

    return true;
}

bool DirScanner::goToNextImDir(){

    while(true){
        
        if(! goToNextDir()){
            return false;
        }
        
        if(ok){
            return true;
        }else{
            continue;
        }
    }
}

bool DirScanner::goToPreviousDir(){

    fs::path tmp;
    if(! previousBrotherDir(tmp, false)){
        return goToParentDir();
    }else{
        goToPreviousBrotherDir();
        while(goToLastChildDir()){
        }
        return true;
    }
}

bool DirScanner::goToPreviousImDir(){

    while(true){

        if(! goToPreviousDir()){
            return false;
        }

        if(ok){
            return true;
        }else{
            continue;
        }
    }
}

bool DirScanner::goToParentDir(){

    fs::path p = cur_dir.parent_path();

    if(p.generic_string().size() == 0){
        return false;
    }

    child_dir_history.push_back(cur_dir);
    cur_dir = p;

    updateEntries();
    findFirstIm();

    return true;
}

bool DirScanner::goToFirstImDirUnderNextParentDir(){

    fs::path parent = cur_dir.parent_path();

    fs::path tmp_cur_dir = cur_dir;

    while(true){
        bool success = goToNextImDir();
        if(! success){
            cur_dir = tmp_cur_dir;
            updateEntries();
            findFirstIm();
            return false;
        }
        if(parent.generic_string() != cur_dir.parent_path().generic_string()){
            return true;
        }
    }
}

bool DirScanner::goToLastImDirUnderPreviousParentDir(){

    fs::path parent = cur_dir.parent_path();

    fs::path tmp_cur_dir = cur_dir;

    while(true){
        bool success = goToPreviousImDir();
        if(! success){
            cur_dir = tmp_cur_dir;
            updateEntries();
            findFirstIm();
            return false;
        }
        if(parent.generic_string() != cur_dir.parent_path().generic_string()){
            return true;
        }
    }
}

bool DirScanner::goToFirstChildDir(){

    for(size_t i = 0; i < entries.size(); i++){
        if(! fs::is_regular_file(fs::path(entries[i]))){
                
            cur_dir = fs::path(entries[i]);
            updateEntries();
            findFirstIm();
            return true;
        }
    }

    return false;
}

bool DirScanner::goToLastChildDir(){

    fs::path path;

    for(auto& itr : entries){
        if(! fs::is_regular_file(fs::path(itr))){

            path = fs::path(itr);
        }
    }

    if(! path.empty()){
        cur_dir = path;
        updateEntries();
        findFirstIm();
        return true;
    }else{
        return false;
    }
}

bool DirScanner::goToChildDirUsingHistory(){

    if(! child_dir_history.empty()){

        cur_dir = child_dir_history.back();
        child_dir_history.pop_back();
        updateEntries();
        findFirstIm();
        return true;
        
    }else{
        return goToFirstChildDir();
    }
}

bool DirScanner::goToNextBrotherDir(){

    child_dir_history.clear();

    fs::path tmp;

    const bool ret = nextBrotherDir(tmp, loop_dir);

    if(ret){
        cur_dir = tmp;
        updateEntries();
        findFirstIm();
    }

    return ret;
}

bool DirScanner::goToPreviousBrotherDir(){

    child_dir_history.clear();

    fs::path tmp;
    const bool ret = previousBrotherDir(tmp, loop_dir);

    if(ret){
        cur_dir = tmp;
        updateEntries();
        findFirstIm();
    }

    return ret;
}


std::string DirScanner::getCurrentDir()const{

    return cur_dir.generic_string();
}

void DirScanner::updateEntries(){

    scanDir(cur_dir, entries);
}

void DirScanner::scanDir(const fs::path& path, std::vector<std::string>& result)const{

    std::vector<std::string> buf;

    for(auto& itr : fs::directory_iterator(path)){
        buf.push_back(itr.path().generic_string());
    }

    std::sort(buf.begin(), buf.end(), &compare_string);

    result = buf;
}

void DirScanner::findFirstIm(){

    for(int i = 0; i < (int)entries.size(); i++){

        if(fs::is_regular_file(fs::path(entries[i])) && isImageFile(entries[i])){
            im_ix = i;
            ok = true;
            return;
        }
    }

    ok = false;
    im_ix = -1;
}

bool DirScanner::nextBrotherDir(fs::path& brother_dir, const bool loop_enabled)const{

    fs::path p = cur_dir.parent_path();

    if(p.generic_string().size() == 0){
        return false;
    }

    std::vector<std::string> contents_of_parent;
    scanDir(p, contents_of_parent);

    bool finish = false;
    fs::path first_found_path;
    for(size_t i = 0; i < contents_of_parent.size(); i++){
        if(first_found_path.empty() && ! fs::is_regular_file(fs::path(contents_of_parent[i]))){
            first_found_path = fs::path(contents_of_parent[i]);
        }
        if(finish){
            if(! fs::is_regular_file(fs::path(contents_of_parent[i]))){
                brother_dir = fs::path(contents_of_parent[i]);
                return true;
            }
        }else{
            if(contents_of_parent[i] == cur_dir.generic_string()){
                finish = true;
            }
        }
    }

    if(finish){
        if(loop_enabled){
            brother_dir = first_found_path;
            return true;
        }else{
            return false;
        }
    }else{
        std::cerr << my_utils_kk4::red
                  << "[ERROR] Unexpected error occured at line " << __LINE__
                  << " in " << __FILE__ << my_utils_kk4::default_style << std::endl;
        return false;
    }
}

bool DirScanner::previousBrotherDir(fs::path& brother_dir, const bool loop_enabled)const{

    fs::path p = cur_dir.parent_path();

    if(p.generic_string().size() == 0){
        return false;
    }

    std::vector<std::string> contents_of_parent;
    scanDir(p, contents_of_parent);

    fs::path prev_path;

    // find last dir
    if(loop_enabled){
        for(size_t i = 0; i < contents_of_parent.size(); i++){
            if(! fs::is_regular_file(fs::path(contents_of_parent[i]))){
                prev_path = fs::path(contents_of_parent[i]);
            }
        }
    }
    
    for(size_t i = 0; i < contents_of_parent.size(); i++){
        
        if(contents_of_parent[i] == cur_dir.generic_string()){
            
            if(prev_path.empty()){
                return false;
            }else{
                brother_dir = prev_path;
                return true;
            }

        }else if(! fs::is_regular_file(fs::path(contents_of_parent[i]))){
            prev_path = fs::path(contents_of_parent[i]);
        }
    }

    std::cerr << my_utils_kk4::red
              << "[ERROR] Unexpected error occured at line " << __LINE__
              << " in " << __FILE__ << my_utils_kk4::default_style << std::endl;
    
    return false;
}


bool DirScanner::isImageFile(const std::string& path_str){

    fs::path fspath(path_str);

    if(! fs::is_regular_file(fspath)){
        return false;
    }

    std::string ext = fspath.extension().generic_string();

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

bool DirScanner::compare_string(const std::string& lh, const std::string& rh){

    size_t min_size = lh.size() > rh.size() ? rh.size() : lh.size();

    for(size_t i = 0; i < min_size; i++){
        if(lh[i] != rh[i]){
            return lh[i] < rh[i];
        }
    }

    if(lh.length() != rh.length()){
        return lh.length() < rh.length();
    }else{
        return false;
    }
}



