#include "TaskOutputer.h"

void LogOutputer::operator()(ThreadQueueCondition<Task>& q) {
    bool done_flag = false;
    Task task_;
    while(!done_flag) {
        std::cout << "waiting ... (Outputer)\n"; 
        if (q.wait_and_pop(task_, pop_condition_fun, done_flag)) // - Is it "if" really need?  - Yes, for threatment exit by done_flag
            std::cout << "Output: " << task_.str << std::endl;
    }
}