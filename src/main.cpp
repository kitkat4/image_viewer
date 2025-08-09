#include "image_viewer.hpp"

#include <string>
#include <iostream>
#include <exception>

int main(int argc, char* argv[]){

    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <image file or directory>" << std::endl;
        return 1;
    }

    ImageViewer iv;
    if(! iv.init(argv[1])){
        std::cerr << "Error: no image found" << std::endl;
        return 1;
    }

    iv.enterMainLoop();

    return 0;
}

