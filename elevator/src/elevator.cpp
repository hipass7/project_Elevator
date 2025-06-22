#include "elevator.h"
#include <iostream>
#include <thread>
#include <chrono>

Elevator::Elevator(const std::string& config_path)
    : config(ElevatorConfig::loadFromFile(config_path)), can_interface(config), current_floor(0) {
    std::cout << "[Elevator] Initialized elevator ID " << config.id << "\n";
}

void Elevator::run() {
    std::cout << "[Elevator] Running elevator logic...\n";
    while (true) {
        auto openDoor = can_interface.receiveControlCommand();
        can_interface.sendElevatorStatus(current_floor);
        // 시뮬레이션용 floor 변경 로직
        current_floor = (current_floor + 1) % 20;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}