#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <exception>

// #include <qapplication.h>
// #include <qwidget.h>


int main(int argc, char* argv[]){

    std::string window_name("test");
    
    std::string path(argv[1]);

    cv::Mat frame = cv::imread(path);
    
    cv::namedWindow(window_name);

    cv::imshow(window_name, frame);

    while(true){

        char c = cv::waitKey(10);

        // if(c != -1)
        //     std::cout << c << std::endl;
        if(c == 'S'){
            std::cout << "next image" << std::endl;
        }else if(c == 'Q'){
            std::cout << "previous image" << std::endl;
        }
        
        try{
            if(cv::getWindowProperty(window_name, 0) < 0)
                break;
        }catch(std::exception){
            break;
        }
    }

    return 0;
}

