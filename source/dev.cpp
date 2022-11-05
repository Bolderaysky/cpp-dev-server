#include "dev.hpp"


bool dev::ReadConfig() {

    std::ifstream config(args.at("cfg").get<std::string>(), std::ios_base::in);

    if (config.is_open()) {

        std::cout << "Config was successfully opened. Reading config...\n";
        cfg = json::parse(config);
        config.close();
        std::cout << "Config was successfully read.\n";

        return true;

    } else {

        std::cout << "Config cannot be read. Check if config exists or if you "
                     "have permissions to read config.\n";
    }

    return false;
}

void dev::Execute() {

    using namespace water::watcher;

    const auto lambda = [=](json j) {
        std::uint64_t pid;

        json actions = j.at("onChange");
        for (auto steps = actions.begin(); steps != actions.end(); ++steps) {

            if ((*steps).get<std::string>() != "rerun") {

                system((*steps).get<std::string>().c_str());
            } else {

                if ((pid = fork()) == 0) {

                    execl(j.at("project").get<std::string>().c_str(),
                          j.at("project").get<std::string>().c_str(), "", NULL);
                }
            }
        }

        watch(j.at("path").get<std::string>().c_str(),
              [=](const event::event& this_event) mutable {
                  for (auto steps = actions.begin(); steps != actions.end();
                       ++steps) {

                      if ((*steps).get<std::string>() != "rerun") {

                          system((*steps).get<std::string>().c_str());
                      } else {

                          kill(pid, SIGKILL);

                          if ((pid = fork()) == 0) {

                              execl(j.at("project").get<std::string>().c_str(),
                                    j.at("project").get<std::string>().c_str(),
                                    "", NULL);
                          }
                      }
                  }
              });
    };

    for (auto it = cfg.begin(); it != cfg.end(); ++it) {
        std::cout << "------------------------------\n"
                  << "Name: " << (*it)["name"] << '\n'
                  << "Path: " << (*it)["path"] << '\n'
                  << "Project: " << (*it)["project"] << '\n'
                  << "------------------------------\n";

        std::thread(lambda, std::move(*it)).detach();
    }
}

void dev::ParseArguments(std::uint64_t argc, char* argv[]) noexcept {

    std::vector<std::string>* tmp = new std::vector<std::string>;

    std::string* key = new std::string;
    std::string* value = new std::string;

    tmp->assign(argv + 1, argv + argc);
    for (auto x : *tmp) {

        *key = x.substr(0, x.find("="));
        *value = x.substr(x.find("=") + 1, x.length() - 1);
        args[*key] = *value;
    }

    if (!args.contains("cfg")) {
        args["cfg"] = "dev.json";
        std::cout << "Config was not provided. Using default settings...\n";
    }


    delete key;
    delete value;
    delete tmp;
}

std::uint64_t dev::loop() {

    if (!ReadConfig()) return -1;
    Execute();

    for (;;) std::this_thread::sleep_for(std::chrono::seconds(60));

    return 0;
}