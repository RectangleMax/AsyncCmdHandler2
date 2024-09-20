#pragma once
#include <fstream>
#include <thread>
#include <vector>

#include "ThreadQueueCondition.h"
#include "TaskOutputer.h"

class TaskHandler {
    ThreadQueueCondition<Task> q;
    std::thread output_thread;
    std::vector<std::thread> threads_vec;
public:
    void push(std::string&&);
    // void registerOutputThreads(BaseOutputer*);
    void startOutputThreads(LogOutputer, int);
    void wake_up_and_done();
    void joinOutputThreads();

    ~TaskHandler() { 
        std::cout << "Destructor of TaskHandler is working ..." << std::endl; 
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