#!/usr/bin/bash

clang++ main.cpp asyncHandler.cpp TaskHandler.cpp TaskOutputer.cpp -fsanitize=thread -std=c++20
./a.out 3 input1.txt
