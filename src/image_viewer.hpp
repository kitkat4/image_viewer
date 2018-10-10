#pragma once


#include <opencv2/opencv.hpp>

#include <experimental/filesystem>
#include <string>
#include <vector>
#include <iostream>

namespace fs = std::experimental::filesystem;

class ImageViewer{
public:

    ImageViewer(const std::string& path);
    ~ImageViewer();
    operator bool()const{
        return ok;
    }

    static void enterMainLoop();

    
protected:

    ImageViewer* mainLoop();
    
    static const std::string window_name;
    static bool window_opened;
    static ImageViewer* cur_instance;


    bool ok;

    cv::Mat cur_im;

    std::vector<std::string> entries;
    
    fs::path cur_dir;
    int im_ix;
};

