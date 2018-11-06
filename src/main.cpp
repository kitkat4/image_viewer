#include "image_viewer.hpp"

#include <string>
#include <iostream>
#include <exception>

int main(int argc, char* argv[]){

    ImageViewer iv(argv[1]);

    iv.enterMainLoop();

    return 0;
}

