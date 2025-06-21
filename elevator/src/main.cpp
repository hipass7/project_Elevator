#include "elevator.h"
#include "manifest.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string config_path = "config/ev_config.json";
    if (argc > 1) {
        config_path = argv[1];
    }

    ManifestLoader loader("config_path");
    const ElevatorConfig& config = loader.getConfig().elevators[0];  // 예: 첫 번째 엘리베이터

    Elevator elevator(config);
    elevator.run();

    return 0;
}