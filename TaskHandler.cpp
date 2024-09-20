#include "TaskHandler.h"


void TaskHandler::push(std::string&& cmd) {
    q.push(Task{cmd, 2});
}

// void TaskHandler::startOutputThread(Outputer o) {
//     output_thread = std::thread{o, std::ref(q)};
// }

void TaskHandler::startOutputThreads(LogOutputer o, int num_threads) {
    for (int i = 0; i < num_threads; ++i)
        threads_vec.emplace_back(o, std::ref(q));
}

void TaskHandler::wake_up_and_done() {
    q.wake_up_and_done();
}


