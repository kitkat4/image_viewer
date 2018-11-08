#pragma once

#include "image_viewer.hpp"

#include <opencv2/opencv.hpp>
#include <experimental/filesystem>

#include <string>
#include <vector>
#include <iostream>

namespace fs = std::experimental::filesystem;

class ImageViewer;

class DirScanner{
    friend class ImageViewer;
protected:

    DirScanner(const std::string& path, const std::string& window_name,
               ImageViewer* const viewer);
    ~DirScanner();
    operator bool()const{
        return ok;
    }
    DirScanner* mainLoop();

    std::string nextBrotherDir()const;
    std::string previousBrotherDir()const;
    static bool isImageFile(const std::string& path_str);

    const std::string window_name;

    bool ok;

    

    std::vector<std::string> entries;
    
    fs::path cur_dir;
    int im_ix;

    ImageViewer* const viewer;
};


