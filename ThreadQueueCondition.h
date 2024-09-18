#pragma once
#include <iostream>
#include <list>
#include <condition_variable>
#include <functional>


template<typename T>
class ThreadQueueCondition {
    std::mutex mut;
    std::condition_variable cond_var;
    std::list<T> queue;

    std::mutex replace_for_atomic_mutex;
    bool done_flag = false;
public:
    void wake_up_and_done();
    void push(T t);
    bool wait_and_pop(T& t, std::function<bool(T&)> pop_condition);
    bool wait_and_pop();

    size_t size() { return queue.size(); }
};

#include "ThreadQueueCondition.cpp"