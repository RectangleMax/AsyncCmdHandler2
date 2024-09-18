#pragma once
#include "ThreadQueueCondition.h"
#include <fstream>
#include <thread>

struct Outputer {
    void operator()(ThreadQueueCondition<std::string>&);
};

class EasyHandler {
    ThreadQueueCondition<std::string> q;
    std::thread output_thread;
public:
    void push(std::string&);
    void startOutputThread(Outputer);
    void startOutputThread();
    void wake_up_and_done();
    
    ~EasyHandler() { 
        std::cout << "Destructor of EasyHandler is working ..." << std::endl; 
        // output_thread.join();
    }

    void destroy() {
        // wake_up_and_done();
        // std::this_thread::sleep_for(std::chrono::microseconds(2000));

        std::cout << "Join thread before." << std::endl;        
        output_thread.join();        
        std::cout << "Join thread after." << std::endl;        
    }
};