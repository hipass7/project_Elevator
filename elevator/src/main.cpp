#include "elevator.h"
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[]) {
    std::string config_path = "../config/ev_config.json";
    if (argc > 1) {
        config_path = argv[1];
    }

    try {
        Elevator elevator(config_path);
        elevator.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}