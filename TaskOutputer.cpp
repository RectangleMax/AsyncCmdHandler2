#include "TaskOutputer.h"

void BaseOutputer::common_output(ThreadQueueCondition<Task>& q, int flag_index,
        std::function<bool(Task&)> pop_condition_lambda, std::function<void(Task&)> spec_output_lambda) {
    bool done_flag = false;
    while(!done_flag) {
        Task task_;
        q.wait_and_pop(task_, pop_condition_lambda, done_flag); // - Is it "if" really need?  - Yes, for threatment exit by done_flag
        if (task_.need_outputer_flags[flag_index]) {
            spec_output_lambda(task_);
        }
    }
}

void LogOutputer::operator()(ThreadQueueCondition<Task>& q) {
    common_output(q, flag_index, pop_condition_fun(), 
        [] (Task& task_) {
            std::cout << task_.cmd << std::endl;
        }
    );
}

void FileOutputer::operator()(ThreadQueueCondition<Task>& q) {
    common_output(q, flag_index, pop_condition_fun(), 
        [] (Task& task_) {
            std::ofstream file(task_.fileName);
            file << task_.cmd;
        }
    );
}