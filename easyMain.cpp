#include "asyncHandler.h"
#include <iostream>

int main(int argc, char** argv) {
    int num_threads;
    if (argc < 2)
        num_threads = 1;
    else
        num_threads = std::stoi(argv[1]);

    std::cout << std::endl << "start" << std::endl;

    connect(num_threads);

    receive("a1");
    receive("a2");

    disconnect();

    return 0;
}