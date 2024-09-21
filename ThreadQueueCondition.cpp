
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
void ThreadQueueCondition<T>::push(T&& t) {
    std::unique_lock ulock{mut};
    queue.push_back(t);
    
    // std::cout << "size " << queue.size() << std::endl;

    // std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    ulock.unlock();
    cond_var.notify_one();
}

template<typename T>
void ThreadQueueCondition<T>::wait_and_pop(T& task_, std::function<bool(T&)> pop_condition_fun, bool& done_outside_flag) {
    std::unique_lock ulock{mut};
    cond_var.wait(ulock, [this] {return !queue.empty() || done_flag;});

    if (!queue.empty()) {
        task_ = queue.front();

        // std::cout << task_.str << std::endl;

        if (pop_condition_fun(queue.front())) {
            queue.pop_front();
        }
        ulock.unlock();
        cond_var.notify_one();
        return; // todo: delete this return?
    } 
    if (done_flag) {
        done_outside_flag = true;
    }
}
