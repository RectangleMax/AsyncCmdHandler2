#include "EasyHandler.h"

void fun(ThreadQueueCondition<std::string>& q) {
    
    bool flag = true;
    while(flag) {

        std::cout << "waiting ... (Outputer)\n"; 

        std::string s;
        flag = q.wait_and_pop(s);

        // std::ofstream file("file_" + s + ".txt");
        // file << s;
        
        if (!s.empty()) {
            std::cout << "Output: " << s << std::endl;
        }
    }
}


void EasyHandler::push(std::string& cmd) {
    q.push(std::move(cmd));
}

void EasyHandler::startOutputThread(Outputer o) {
    output_thread = std::thread{o, std::ref(q)};
}

void EasyHandler::startOutputThread() {
    output_thread = std::thread{fun, std::ref(q)};
}

void EasyHandler::wake_up_and_done() {
    q.wake_up_and_done();
}


void Outputer::operator()(ThreadQueueCondition<std::string>& q) {
    
    bool flag = true;
    while(flag) {

        std::cout << "waiting ... (Outputer)\n"; 

        std::string s;
        flag = q.wait_and_pop(s);

        // std::ofstream file("file_" + s + ".txt");
        // file << s;
        
        if (!s.empty()) {
            std::cout << "Output: " << s << std::endl;
        }
    }
}

