#include "TaskOutputer.h"

void LogOutputer::operator()(ThreadQueueCondition<Task>& q) {
    bool done_flag = false;
    while(!done_flag) {

        // std::cout << "waiting (log)\n";
        Task task_;
        q.wait_and_pop(task_, pop_condition_fun(), done_flag); // - Is it "if" really need?  - Yes, for threatment exit by done_flag
        if (task_.need_outputer_flags[flag_index])
            std::cout << "LogOutput: " << task_.str << std::endl;
    }
}

void FileOutputer::operator()(ThreadQueueCondition<Task>& q) {
    bool done_flag = false;
    while(!done_flag) {

        // std::cout << "waiting (file)\n";
        Task task_;
        q.wait_and_pop(task_, pop_condition_fun(), done_flag); // - Is it "if" really need?  - Yes, for threatment exit by done_flag
        if (task_.need_outputer_flags[flag_index])
            std::cout << "FileOutput: " << task_.str << std::endl;
    }
}