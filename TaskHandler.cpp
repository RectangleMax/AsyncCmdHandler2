#include "TaskHandler.h"

void TaskHandler::registerOutputer(BaseOutputer* ptr, int num_threads) {
    outputers_ptr.push_back({ptr, num_threads});
}

void TaskHandler::startOutputThreads() {
    for (auto pair_ : outputers_ptr) {
        for (int i = 0; i < pair_.second; ++i) {
            threads_vec.emplace_back(std::ref(*pair_.first), std::ref(q));
        }
    }
    task_pattern = Task{std::vector<bool>{true, true}};
}

void TaskHandler::wake_up_and_done() {
    q.wake_up_and_done();
}

std::string TaskHandler::getFileName(time_t t) {
    auto it = uniqueTimesCounter.find(t);
    std::string postfix;
    if (it == uniqueTimesCounter.end()) {
        uniqueTimesCounter.insert({t, 0});
    } else {
        postfix = "_" + std::to_string(++it->second);
    }
    return "bulk" + std::to_string(t) + postfix + ".log";
}

void TaskHandler::processing(std::string& cmd, size_t id, time_t time_) {
    auto it = connections_map.find(id);
    Connection& con = it->second;
    if (cmd == "{") {
        if ((!con.bracket_counter++) && (!con.cmd_block.empty())) { 
            q.push(Task{task_pattern, std::move(con.cmd_block), con.fileName});
            con.cmd_block.clear();
            con.n_pack_cur = 0;
        }
        return;
    } else if (cmd == "}") {
        if (!--con.bracket_counter) {
            q.push(Task{task_pattern, std::move(con.cmd_block), con.fileName});
            con.cmd_block.clear();
            con.n_pack_cur = 0;
        }
        return;
    } 
    if (con.cmd_block.empty()) {
        con.cmd_block = cmd;
        con.fileName = getFileName(time_);
    } else {
        con.cmd_block.append(", " + cmd);
    }
    ++(con.n_pack_cur);
    if (!con.bracket_counter) {
        if (con.n_pack_cur == con.N_pack) {
            q.push(Task{task_pattern, std::move(con.cmd_block), con.fileName});
            con.cmd_block.clear();
            con.n_pack_cur = 0;
        }
    }
}

