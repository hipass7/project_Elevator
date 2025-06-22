#include "elevator.h"
#include "elevator_config.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string config_path = "config/ev_config.json";
    if (argc > 1) {
        config_path = argv[1];
    }

    Elevator elevator(config_path);
    elevator.run();

    return 0;
}