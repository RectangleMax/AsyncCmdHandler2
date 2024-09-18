#include "EasyAsync.h"
#include "EasyHandler.h"
#include <fstream>

EasyHandler hand;


void connect(int num_threads) {
    hand.startOutputThreads(Outputer{}, num_threads);
}

void receive(std::string cmd) {
    hand.push(cmd);
}

void disconnect() {
    hand.wake_up_and_done();
    hand.destroy();
}