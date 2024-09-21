#include "asyncHandler.h"
#include <iostream>
#include <algorithm>

int main(int argc, char** argv) {
    int num_threads;
    if (argc < 2)
        num_threads = 1;
    else
        num_threads = std::stoi(argv[1]);

    std::cout << std::endl << "start" << std::endl;


    std::vector<int> id_vec {connect(3), connect(2)};
    std::vector<std::vector<std::string>> cmd_groups{
            {"a1", "a2", "a3", "a4", "a5"}, 
            {"b1", "b2", "{", "b3", "b4", "}", "{", "b5", "b6", "{", "b7", "b8", "}", "b9", "}", "{", "b10", "b11"}
    };
    std::vector<size_t> counters {cmd_groups[0].size(), cmd_groups[1].size()};
    while (*std::max_element(counters.begin(), counters.end())) {
        for (int i = 0; i < cmd_groups.size(); ++i) {
            if (counters[i] == 0)
                continue;
            //std::cout << cmd_groups[i][cmd_groups[i].size() - counters[i]] << std::endl;
            receive(cmd_groups[i][cmd_groups[i].size() - counters[i]], id_vec[i]);
            if (--counters[i] == 0)
                disconnect(id_vec[i]);
        }
    }


    return 0;
}