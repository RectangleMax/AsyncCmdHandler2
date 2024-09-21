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


