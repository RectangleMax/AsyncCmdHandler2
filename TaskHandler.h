#pragma once
#include <fstream>
#include <thread>
#include <vector>
#include <map>

#include "ThreadQueueCondition.h"
#include "TaskOutputer.h"

using size_t = std::size_t;

struct Connection {
    // std::string coutput_cmd;
    size_t N_pack;
    std::string cmd_block;
    size_t bracket_counter = 0;
    std::string fileName;
    size_t n_pack_cur = 0;
    Connection(size_t n) : N_pack(n) {}
};


class TaskHandler {
    ThreadQueueCondition<Task> q;
    Task task_pattern;

    std::vector<std::thread> threads_vec;

        // std::vector<BaseOutputer*> p_outputer_list;
    
    inline static int connections_counter = -1;
    std::map<int, Connection> connections_map;
    
    std::string getFileName(time_t);
    std::map<time_t, int> uniqueTimesCounter;
public:
    void processing(std::string&, size_t, time_t);
    void startOutputThreads(LogOutputer, int, FileOutputer, int);
    void wake_up_and_done();
    void joinOutputThreads();

    ~TaskHandler() { 
        std::cout << "Destructor of TaskHandler is working ..." << std::endl; 
        wake_up_and_done();
        stoptOutputThreads();
    }
private:
    void stoptOutputThreads() {
        // wake_up_and_done();
        // std::this_thread::sleep_for(std::chrono::microseconds(2000));

        std::cout << "Join thread before." << std::endl;        
        for (auto& t : threads_vec) {
            t.join();
        }
        std::cout << "Join thread after." << std::endl;        
    }

public:
    int add_connection(size_t N_pack) {
        connections_map.insert({++connections_counter, Connection{N_pack}});
        return connections_counter;
    }

    void del_connection(int id) {
        auto it = connections_map.find(id);
        if (it->second.n_pack_cur && !it->second.bracket_counter)
            q.push(Task{task_pattern, std::move(it->second.cmd_block), it->second.fileName});
        connections_map.erase(it);
    }

};