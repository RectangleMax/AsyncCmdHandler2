#include "EasyHandler.h"


void EasyHandler::push(std::string& cmd) {
    q.push(std::move(cmd));
}

// void EasyHandler::startOutputThread(Outputer o) {
//     output_thread = std::thread{o, std::ref(q)};
// }

void EasyHandler::startOutputThreads(Outputer o, int num_threads) {
    for (int i = 0; i < num_threads; ++i)
        threads_vec.emplace_back(o, std::ref(q));
}

void EasyHandler::wake_up_and_done() {
    q.wake_up_and_done();
}

void Outputer::operator()(ThreadQueueCondition<std::string>& q) {
    while(true) {
        std::cout << "waiting ... (Outputer)\n"; 
        if (!q.wait_and_pop())
            return;
    }
}

