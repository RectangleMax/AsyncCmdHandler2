#include "EasyAsync.h"
#include "EasyHandler.h"
#include <fstream>

EasyHandler hand;


void connect() {
    // hand.startOutputThread(Outputer{});
    hand.startOutputThread();
}

void receive(std::string cmd) {
    hand.push(cmd);
}

void disconnect() {
    hand.wake_up_and_done();
    hand.destroy();
}