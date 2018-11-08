#pragma once

#include <chrono>
#include <string>
#include <iostream>

namespace my_utils_kk4{


static const std::string default_style = "\033[0m"; 
static const std::string bold    = "\033[1m";
static const std::string italic  = "\033[3m";
static const std::string underline = "\033[4m";

static const std::string black   = "\033[30m";
static const std::string red     = "\033[31m";
static const std::string green   = "\033[32m";
static const std::string yellow  = "\033[33m";
static const std::string blue    = "\033[34m";
static const std::string magenta = "\033[35m";
static const std::string cyan    = "\033[36m";
static const std::string white   = "\033[37m";
static const std::string default_color  = "\033[39m";
static const std::string black_b   = "\033[40m";
static const std::string red_b     = "\033[41m";
static const std::string green_b   = "\033[42m";
static const std::string yellow_b  = "\033[43m";
static const std::string blue_b    = "\033[44m";
static const std::string magenta_b = "\033[45m";
static const std::string cyan_b    = "\033[46m";
static const std::string white_b   = "\033[47m";
static const std::string default_color_b  = "\033[49m";



void progBarNh(const double progress, const bool first_call);


class Fps{
public:
    Fps(double update_interval = 1);
    ~Fps();
    void informEvent();
    double getFps()const{
        return fps;
    }
private:
    std::chrono::system_clock::time_point time_of_last_event;
    std::chrono::microseconds elapsed_time_after_last_update;
    int events_num_after_last_update;
    bool no_events_yet;
    int update_interval;
    double fps;
};


class StopWatch{
public:
    StopWatch();
    ~StopWatch();
    void start();
    double stop();
    void reset();
    double lap();
private:
    bool measuring;
    std::chrono::system_clock::time_point time_start;
    std::chrono::microseconds offset;

};




}


// sources
namespace my_utils_kk4{

inline void progBarNh(const double progress, const bool first_call){

    double prog;
    if(progress < 0.0){
        prog = 0.0;
    }else if(progress > 1.0){
        prog = 1.0;
    }else{
        prog = progress;
    }

    const char * nh[] = {"           ／⌒ヽ                  ",
                         "⊂二二二（  ＾ω＾）二⊃           ",
                         "                /             ﾌﾞｰﾝ ",
                         "        （  ヽノ                   ",
                         "         ﾉ>ノ                      ",
                         "   三    レﾚ                       "};

    
    std::string offset((int)(prog * 40), ' ');

    int prog_per = (int)(prog * 100);

    if(! first_call){
        std::cout << "\033[6A\r";
    }

    for(int i = 0; i < 6; i++){
        std::cout << "|" << offset << nh[i];
        if(i == 0){
            std::cout << "\r\033[64C| " << prog_per << " %          \n";
        }else{
            std::cout << "\r\033[64C|           \n";
        }
    }

    std::cout << std::flush;
    return;
}

// Constructor
inline Fps::Fps(double update_interval)
    : no_events_yet(true),
      events_num_after_last_update(0),
      elapsed_time_after_last_update(std::chrono::microseconds(0)),
      update_interval((int)(update_interval * 1000000)),
      fps(0.0){
}

// Destructor
inline Fps::~Fps(){
}


inline void Fps::informEvent(){

    
    if(no_events_yet){          // fist call
        no_events_yet = false;
        time_of_last_event = std::chrono::system_clock::now();
    }else{                      // calls after the second
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

        elapsed_time_after_last_update +=
            std::chrono::duration_cast<std::chrono::microseconds>(now - time_of_last_event);

        time_of_last_event = now;

        events_num_after_last_update++;
    }

    if(elapsed_time_after_last_update.count() > update_interval){
        fps = 1000000 * (double) events_num_after_last_update / elapsed_time_after_last_update.count();
        elapsed_time_after_last_update = std::chrono::microseconds(0);
        events_num_after_last_update = 0;
    }
    
    return;
}


inline StopWatch::StopWatch()
    : measuring(false),
      offset(std::chrono::microseconds(0))
{}

inline StopWatch::~StopWatch(){}


inline void StopWatch::start(){
    if(! measuring){
        measuring = true;
        time_start = std::chrono::system_clock::now();
    }else
        std::cerr << "[ERROR] " << __FILE__ << " (line " << __LINE__ << "): StopWatch::start() called while already running" << std::endl;
}

inline double StopWatch::stop(){
    if(measuring){
        auto elapsed = std::chrono::system_clock::now() - time_start;
        auto result = std::chrono::duration_cast<std::chrono::microseconds>(elapsed) + offset;
        offset = result;
        measuring = false;
        return result.count() / (double) 1000000;
    }else
        std::cerr << "[ERROR] " << __FILE__ << " (line " << __LINE__ << "): StopWatch::stop() called while not running" << std::endl;
}



inline void StopWatch::reset(){
    if(! measuring)
        offset = std::chrono::microseconds(0);
    else
        std::cerr << "[ERROR] " << __FILE__ << " (line " << __LINE__ << "): StopWatch::reset() called while running" << std::endl;
}

inline double StopWatch::lap(){
    if(measuring){
        auto elapsed = std::chrono::system_clock::now() - time_start;
        auto result = std::chrono::duration_cast<std::chrono::microseconds>(elapsed) + offset;
        return result.count() / (double) 1000000;
    }else
        std::cerr << "[ERROR] " << __FILE__ << " (line " << __LINE__ << "): StopWatch::lap() called while not running" << std::endl;
    return 0;
}


}


