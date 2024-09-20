#!/usr/bin/bash
if [ "$#" -ne 2 ]; then
    echo "Need two input argument: 1 - num of threads, 2 - num of run"
    exit
fi
if [ -f temp.txt ]; then
    rm temp.txt;
fi

clang++ easyMain.cpp asyncHandler.cpp TaskHandler.cpp TaskOutputer.cpp
for ((i = 0; i < $2; ++i)); do { ./a.out $1; } &>>temp.txt; done
grep -c -i "seg" temp.txt

