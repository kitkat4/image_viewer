#pragma once

#include "window_manager.hpp"
#include "my_utils_kk4.hpp"

#include <opencv2/opencv.hpp>
#include <experimental/filesystem>

#include <string>
#include <vector>
#include <iostream>

namespace fs = std::experimental::filesystem;
using Command = WindowManager::Command;

class DirScanner{
public:
    DirScanner(const std::string& path);
    ~DirScanner();

    std::string getCurrentIm()const;
    std::string goToNextIm();
    std::string goToPreviousIm();
    void goToParentDir();
    void goToChildDir();
    void goToNextBrotherDir();
    void goToPreviousBrotherDir();
    std::string getCurrentDir()const;

    operator bool()const{
        return ok;
    }
    
protected:

    void scanEntries();
    void findFirstIm();

    std::string nextBrotherDir()const;
    std::string previousBrotherDir()const;
    static bool isImageFile(const std::string& path_str);
    static bool compare_string(const std::string& lh, const std::string& rh);

    bool ok;

    std::vector<std::string> entries;
    
    fs::path cur_dir;
    std::vector<fs::path> child_dir_history;
    
    int im_ix;
};


