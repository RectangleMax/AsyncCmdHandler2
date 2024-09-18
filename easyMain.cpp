#include "EasyAsync.h"
#include <iostream>

int main() {

    std::cout << std::endl << "start" << std::endl;

    connect();

    receive("a1");
    receive("a2");

    disconnect();

    return 0;
}