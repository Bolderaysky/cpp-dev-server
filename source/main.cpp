#include <iostream>

#include "dev.hpp"


int main(int argc, char* argv[]) {

    dev* DevServer = new dev;
    DevServer->ParseArguments(argc, argv);

    return DevServer->loop();
}