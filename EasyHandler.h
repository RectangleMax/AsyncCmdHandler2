#pragma once
#include "ThreadQueueCondition.h"
#include <fstream>
#include <thread>
#include <vector>

struct Outputer {
    void operator()(ThreadQueueCondition<std::string>&);
};

class EasyHandler {
    ThreadQueueCondition<std::string> q;
//    std::thread output_thread;
    std::vector<std::thread> threads_vec;
public:
    void push(std::string&);
    void startOutputThreads(Outputer, int);
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
        for (auto& t : threads_vec) {
            t.join();
        }
        std::cout << "Join thread after." << std::endl;        
    }
};