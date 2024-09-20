#pragma once
#include <string>
#include <fstream>

#include "TaskHandler.h"


void connect(int);
void receive(std::string&&);
void disconnect();