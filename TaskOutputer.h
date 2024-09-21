#pragma once
#include <algorithm>
#include <fstream>

#include "ThreadQueueCondition.h"

struct Task {
    std::string cmd;
    std::vector<bool> need_outputer_flags{false, false};
    std::string fileName;

    Task(std::vector<bool> flags_pattern) : need_outputer_flags(flags_pattern) {} // delete this
    Task(Task& other, std::string&& cmd_, std::string fileName_) : 
        need_outputer_flags(other.need_outputer_flags), cmd(cmd_), fileName(fileName_) {}
    Task() {}
};

class BaseOutputer {
protected:
    // Task getTaskPattern() {};
    bool check_outputers_flags(Task& task_, int flag_index) {
        task_.need_outputer_flags[flag_index] = false;
        return (std::find(task_.need_outputer_flags.begin(), 
                          task_.need_outputer_flags.end(), true) == 
                          task_.need_outputer_flags.end()) ? true : false; 
    }
    void common_output(ThreadQueueCondition<Task>&, int, std::function<bool(Task&)>, std::function<void(Task&)>);
public:
    virtual void operator()(ThreadQueueCondition<Task>&) = 0;
    virtual std::function<bool(Task&)> pop_condition_fun() = 0;
    virtual ~BaseOutputer() {}  
};

class LogOutputer : public BaseOutputer {
    int flag_index = 0;
public:
    void operator()(ThreadQueueCondition<Task>&) override;
    std::function<bool(Task&)> pop_condition_fun() override {
        return [this] (Task& task_) { return check_outputers_flags(task_, flag_index); };
    }
    ~LogOutputer() {}
};

class FileOutputer : public BaseOutputer {
    int flag_index = 1;
public:
    void operator()(ThreadQueueCondition<Task>&) override;
    std::function<bool(Task&)> pop_condition_fun() override {
        return [this] (Task& task_) { return check_outputers_flags(task_, flag_index); };
    }
    ~FileOutputer() {}
};
