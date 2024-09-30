#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>

#include "TaskHandler.h"

namespace async {
    using id_type = int;
    id_type connect(int);
    void receive(std::string&, id_type);
    void receive(std::string&&, id_type);
    void disconnect(id_type);
}