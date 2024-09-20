#include "asyncHandler.h"

TaskHandler hand;


void connect(int num_threads) {
    hand.startOutputThreads(LogOutputer{}, num_threads);
}

void receive(std::string&& cmd) {
    hand.push(std::move(cmd));
}

void disconnect() {
    hand.wake_up_and_done();
    hand.destroy();
}