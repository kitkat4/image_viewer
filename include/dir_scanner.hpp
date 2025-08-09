#pragma once

#include "window_manager.hpp"

#include <opencv2/opencv.hpp>

#include <filesystem>
namespace fs = std::filesystem;

#include <thread>
#include <future>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using Command = WindowManager::Command;

class DirScanner{
public:
    DirScanner();
    ~DirScanner();

    bool init(const std::string& path);
    
    std::string getCurrentIm()const;
    std::string goToNextIm();
    std::string goToPreviousIm();
    bool goToNextDir();
    bool goToPreviousDir();
    bool goToParentDir();
    bool goToFirstChildDir();
    bool goToLastChildDir();
    bool goToChildDirUsingHistory();
    bool goToNextBrotherDir();
    bool goToPreviousBrotherDir();
    std::string getCurrentDir()const;

    operator bool()const{
        return ok;
    }
    
protected:

    void updateEntries();
    void scanDir(const fs::path& path, std::vector<std::string>& result)const;
    void findFirstIm();

    bool nextBrotherDir(fs::path& brother_dir, const bool loop_enabled)const;
    bool previousBrotherDir(fs::path& brother_dir, const bool loop_enabled)const;
    static bool isImageFile(const std::string& path_str);

    bool ok;

    std::vector<std::string> entries;
    
    fs::path cur_dir;
    std::vector<fs::path> child_dir_history;

    bool loop_dir;
    
    int im_ix;
};


