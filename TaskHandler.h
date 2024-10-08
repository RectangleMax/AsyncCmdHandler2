#pragma once
#include <fstream>
#include <thread>
#include <vector>
#include <map>

#include "ThreadQueueCondition.h"
#include "TaskOutputer.h"

using size_t = std::size_t;

struct Connection {
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
    inline static int connections_counter = -1;
    std::map<int, Connection> connections_map;
    
    std::string getFileName(time_t);
    std::map<time_t, int> uniqueTimesCounter;

    std::vector<std::pair<BaseOutputer*,int>> outputers_ptr;
public:
    void processing(std::string&, size_t, time_t);
    void startOutputThreads();
    void registerOutputer(BaseOutputer*, int);
    void wake_up_and_done();
    void joinOutputThreads();

    ~TaskHandler() { 
        wake_up_and_done();
        stoptOutputThreads();
    }
private:
    void stoptOutputThreads() {
        for (auto& t : threads_vec) {
            t.join();
        }
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