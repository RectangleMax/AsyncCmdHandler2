#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>

#include "TaskHandler.h"


int connect(int);
void receive(std::string&, int);
void receive(std::string&&, int);
void disconnect(int);