#include "asyncHandler.h"

int num_log_threads = 1;
int num_file_threads = 2;


TaskHandler hand;
LogOutputer log_;
FileOutputer file_;

std::once_flag once_flag;
std::mutex mtx;


void register_and_start_outputers() {
    hand.registerOutputer(&log_,  num_log_threads);
    hand.registerOutputer(&file_, num_file_threads);
    hand.startOutputThreads();
}

namespace async {
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
}