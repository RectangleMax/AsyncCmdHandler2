#pragma once
#include "ThreadQueueCondition.h"


struct Task {
    std::string str;
    int output_counter;

    Task(const std::string& str, int x) : str(str), output_counter(x) {}
    Task() {}
};

class BaseOutputer {
protected:
    std::function<bool(Task&)> pop_condition_fun = [this] (Task& t) { return (--t.output_counter) ? false : true; };
public:
    virtual void operator()(ThreadQueueCondition<Task>&) = 0;
    virtual ~BaseOutputer() {}  
};

class LogOutputer : public BaseOutputer {
public:
    void operator()(ThreadQueueCondition<Task>&) override;
    ~LogOutputer() {}
};

