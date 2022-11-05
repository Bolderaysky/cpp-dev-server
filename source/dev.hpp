#pragma once


#ifdef __linux__

#include <sys/types.h>
#include <signal.h>

#endif

#include <string>
#include <fstream>
#include <vector>
#include <chrono>

#include <iostream>

#include "watcher.hpp"
#include "json.hpp"

using json = nlohmann::json;

class dev {

    private:
        json cfg;
        json args;
        bool ReadConfig();
        void Execute();

    public:
        void ParseArguments(std::uint64_t argc, char* argv[]) noexcept;
        std::uint64_t loop();
};