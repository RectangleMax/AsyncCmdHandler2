#include "TaskHandler.h"


void TaskHandler::push(std::string&& cmd) {
    q.push(Task{task_pattern, std::move(cmd)});
}

// void TaskHandler::startOutputThread(Outputer o) {
//     output_thread = std::thread{o, std::ref(q)};
// }

void TaskHandler::startOutputThreads(LogOutputer log_o, int num_log_threads,
                                    FileOutputer file_o, int num_file_threads) {
    for (int i = 0; i < num_log_threads; ++i)
        threads_vec.emplace_back(log_o, std::ref(q));
    for (int i = 0; i < num_file_threads; ++i)
        threads_vec.emplace_back(file_o, std::ref(q));


    task_pattern = Task{std::vector<bool>{true, true}};
}

void TaskHandler::wake_up_and_done() {
    q.wake_up_and_done();
}

// std::string TaskHandler::getFileName(time_t t) {
//     auto it = uniqueTimesCounter.find(t);
//     std::string postfix;
//     if (it == uniqueTimesCounter.end()) {
//         uniqueTimesCounter.insert({t, 0});
//     } else {
//         postfix = "_" + std::to_string(++it->second);
//     }
//     return "bulk" + std::to_string(t) + postfix + ".log";
// }

void TaskHandler::processing(std::string& cmd, size_t id, time_t time_) {
    auto it = connections_map.find(id);
    Connection& con = it->second;


    // std::cout << "id: " << id << ", block: " << con.cmd_block << std::endl;

    // if (!con.fileName.empty())
    //     con.fileName = getFileName(time_); // delete this

    if (cmd == "{") {
        if ((!con.bracket_counter++) && (!con.cmd_block.empty())) { 
            q.push(Task{task_pattern, std::move(con.cmd_block)});
            con.cmd_block.clear();
            con.n_pack_cur = 0;
        }
        return;
    } else if (cmd == "}") {
        if (!--con.bracket_counter) {
            q.push(Task{task_pattern, std::move(con.cmd_block)});
            con.cmd_block.clear();
            con.n_pack_cur = 0;
        }
        return;
    } 
    if (con.cmd_block.empty()) {
        con.cmd_block = cmd;
        // con.fileName = getFileName(time_); uncomment this
    } else {
        con.cmd_block.append(", " + cmd);
    }
    ++(con.n_pack_cur);
    
    // std::cout << "n_pack_cur: " << con.n_pack_cur << std::endl;

    if (!con.bracket_counter) {
        if (con.n_pack_cur == con.N_pack) {

            // std::cout << "Push in processing" << std::endl;

            q.push(Task{task_pattern, std::move(con.cmd_block)});
            con.cmd_block.clear();
            con.n_pack_cur = 0;
        }
    }
}

