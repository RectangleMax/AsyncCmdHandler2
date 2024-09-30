#include "asyncHandler.h"
#include <list>
#include <iostream>
#include <fstream>
#include <thread>


class CmdProducer {
    int N_pack = 0;
    std::list<std::string> cmd_list;
    async::id_type id;
    bool is_connect = false;
public:
    CmdProducer(int N, std::string fileName) : N_pack(N) {
        std::ifstream file{fileName};
        if (!file.is_open()) {
            throw(std::string {"Intput txt-file was not found!\n"});
        }
        for (std::string line; std::getline(file >> std::ws, line); )
            cmd_list.push_back(line);
    }
    CmdProducer() {}
    void operator()() {
        id = async::connect(N_pack);
        is_connect = true;
        while(!cmd_list.empty()) {
            async::receive(cmd_list.front(), id);
            cmd_list.pop_front();
        }
    }
    ~CmdProducer() {
        if (is_connect)
            async::disconnect(id);
    }
};

int main(int argc, char** argv) {
    if (argc != 3 ) {
        std::cerr << "Input argument's list:.\n";
        std::cerr << "1 - size of command's block\n";
        std::cerr << "2 - txt-file with intput command's list." << std::endl;
        return 1; 
    }
    int N_pack = std::stoi(argv[1]);
    std::string fileName {argv[2]};

    CmdProducer prod;
    try {
        prod = CmdProducer {N_pack, fileName};
    } catch (std::string ex_what) {
        std::cout << ex_what;
        return 2;
    }
    std::jthread t1{prod};
    std::jthread t2{prod};
    prod();
}
