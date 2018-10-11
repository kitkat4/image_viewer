#pragma once


#include <opencv2/opencv.hpp>

#include <experimental/filesystem>
#include <string>
#include <vector>
#include <iostream>

namespace fs = std::experimental::filesystem;

bool isImageFile(const std::string& path_str);

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

    const std::string window_name;

    bool ok;

    

    std::vector<std::string> entries;
    
    fs::path cur_dir;
    int im_ix;

    ImageViewer* const viewer;
};



class ImageViewer{
    friend class DirScanner;
public:
    ImageViewer(const std::string& path, const std::string& window_name = "Image Viewer");
    ~ImageViewer();
    void enterMainLoop();

protected:
    const std::string window_name;
    bool window_opened;
    DirScanner* cur_instance;
    cv::Mat cur_im;
    cv::Mat cur_im_original;
    std::string cur_path;
};

