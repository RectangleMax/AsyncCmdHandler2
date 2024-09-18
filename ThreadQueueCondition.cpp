
#include "ThreadQueueCondition.h"

template<typename T>
void ThreadQueueCondition<T>::wake_up_and_done() { 
    std::unique_lock ulock{mut}; // {replace_for_atomic_mutex};
    done_flag = true; 
    ulock.unlock();

    std::cout << "Wake up and done" << std::endl;

    cond_var.notify_all();
}

template<typename T>
void ThreadQueueCondition<T>::push(T t) {
    std::unique_lock ulock{mut};
    queue.push_back(t);
    
    std::cout << "size " << queue.size() << std::endl;

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    ulock.unlock();
    cond_var.notify_one();
}

template<typename T>
bool ThreadQueueCondition<T>::wait_and_pop(T& t, std::function<bool(T&)> pop_condition) {
    std::unique_lock ulock{mut};
    cond_var.wait(ulock, [this] {return !queue.empty() || done_flag;});

    if (done_flag)
        return false;

    t = queue.back();

//    std::cout << "Wait and pop, thread " << std::this_thread::get_id() << ", ready_to pop:" << t.ready_to_pop[0] << ", " << t.ready_to_pop[1] <<  std::endl;

    if (pop_condition(queue.back()))
        queue.pop_back();

    ulock.unlock();

    cond_var.notify_one();
    return true;
}

template<typename T>
bool ThreadQueueCondition<T>::wait_and_pop() {
    std::unique_lock ulock{mut};
    cond_var.wait(ulock, [this] {return !queue.empty() || done_flag;});

    // t = queue.back();

//    std::cout << "Wait and pop, thread " << std::this_thread::get_id() << ", ready_to pop:" << t.ready_to_pop[0] << ", " << t.ready_to_pop[1] <<  std::endl;

    if (!queue.empty()) {
        std::cout << "Output: " << queue.back() << std::endl;
        queue.pop_back();
    } else if (done_flag) {
        return false;
    }

    ulock.unlock();

    cond_var.notify_one();
    return true;
}