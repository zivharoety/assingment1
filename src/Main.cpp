#include "../include/Restaurant.h"
#include <iostream>

using namespace std;

Restaurant *backup = nullptr;

int main(int argc, char **argv) {
 /*  if (argc != 2) {
        std::cout << "usage: rest <config_path>" << std::endl;
        std::cout << argv[0] << endl;
        std::cout << argv[1] << endl;
        std::cout << argv[2] << endl;
        return 0;
    }  */
 std::cout<<"hey"<<std::endl;

    Restaurant rest = Restaurant("/users/studs/bsc/2019/guyof/CLionProjects/untitled/config/config1.txt");
    rest.start();
}

/**
rest.start();
if(backup!=nullptr){
    delete backup;
    backup = nullptr;
}
return 0;
//}
 **/