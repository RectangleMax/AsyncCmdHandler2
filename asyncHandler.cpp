#include "asyncHandler.h"

TaskHandler hand;
std::once_flag once_flag;
std::mutex mtx;

int num_log_threads = 2;
int num_file_threads = 2;

void register_and_start_outputers() {
    hand.startOutputThreads(LogOutputer{}, num_log_threads, FileOutputer{}, num_file_threads);
}

id_type connect(int N_pack) {
    std::call_once(once_flag, register_and_start_outputers);
    int counter;
    {
        std::lock_guard<std::mutex> lock{mtx};
        counter = hand.add_connection(N_pack);
    }
    return counter;
}

void receive(std::string& cmd, id_type id) {
    std::lock_guard<std::mutex> lock{mtx};
    hand.processing(cmd, id, time(0));
}

void receive(std::string&& cmd, id_type id) {
    std::lock_guard<std::mutex> lock{mtx};
    hand.processing(cmd, id, time(0));
}

void disconnect(id_type id) {
    std::lock_guard<std::mutex> lock{mtx};
    hand.del_connection(id);
}