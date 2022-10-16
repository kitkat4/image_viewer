#pragma once

#include "window_manager.hpp"
#include "my_utils_kk4.hpp"

#include <opencv2/opencv.hpp>

#ifdef BUILD_WITH_BOOST_FILESYSTEM
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#include <thread>
#include <future>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

using Command = WindowManager::Command;

class DirScanner{
public:
    DirScanner(const std::string& path);
    ~DirScanner();

    std::string getCurrentIm()const;
    std::string goToNextIm();
    std::string goToPreviousIm();
    bool goToNextDir();
    bool goToNextImDir();
    bool goToPreviousDir();
    bool goToPreviousImDir();
    bool goToParentDir();
    bool goToFirstImDirUnderNextParentDir();
    bool goToLastImDirUnderPreviousParentDir();
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
    static bool compareString(const std::string& lh, const std::string& rh);
    static void compareByNumber(const std::string& lh, const std::string& rh, bool& success, bool& result);
    static bool isNumber(const char c);
    

    bool ok;

    std::vector<std::string> entries;
    
    fs::path cur_dir;
    std::vector<fs::path> child_dir_history;

    bool loop_dir;
    
    int im_ix;
};


