#include "image_viewer.hpp"

#include <string>
#include <iostream>
#include <exception>

int main(int argc, char* argv[]){

    ImageViewer iv(argv[1]);

    ImageViewer::enterMainLoop();

    return 0;
}

// int main(int argc, char* argv[]){

//     std::string window_name("test");
    
//     // std::string path(argv[1]);
    

//     cv::Mat frame = cv::imread(im_path.c_str());

//     if(frame.empty()){
//         std::cerr << "[ERROR] Failed to open " << im_path.c_str() << std::endl;
//         return 1;
//     }

//     std::filesystem::path curr_dir = im_path.parent_path();

    
    
//     cv::namedWindow(window_name);

//     cv::imshow(window_name, frame);

//     while(true){

//         char c = cv::waitKey(10);

//         if(c == 'S'){
//             std::cout << "next image" << std::endl;
//         }else if(c == 'Q'){
//             std::cout << "previous image" << std::endl;
//         }
        
//         try{
//             if(cv::getWindowProperty(window_name, 0) < 0)
//                 break;
//         }catch(std::exception){
//             break;
//         }
//     }

//     return 0;
// }

