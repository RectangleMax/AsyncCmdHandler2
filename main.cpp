#include "asyncHandler.h"
#include <list>
#include <iostream>
#include <fstream>
#include <thread>

class CmdProducer {
    size_t N_pack = 0;
    std::list<std::string> cmd_list;
    id_type id;
    bool is_connect = false;
public:
    CmdProducer(size_t N, std::string fileName) : N_pack(N) {
        std::ifstream file(fileName);
        for (std::string line; std::getline(file >> std::ws, line); )
            cmd_list.push_back(line);
    }
    void operator()() {
        id = connect(N_pack);
        is_connect = true;
        while(!cmd_list.empty()) {
            receive(cmd_list.front(), id);
            cmd_list.pop_front();
        }
    }
    ~CmdProducer() {
        if (is_connect)
            disconnect(id);
    }
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "One input argument need.\n";
        return 1; 
    }
    int N_pack = std::stoi(argv[1]);

    CmdProducer prod(N_pack, "input1.txt");
    std::jthread t1{prod};
    std::jthread t2{prod};
    prod();
}
