#include "elevator.h"
#include <iostream>
#include <thread>
#include <chrono>

Elevator::Elevator(const ElevatorConfig& config)
    : config(config), can_interface(config), current_floor(0) {
    std::cout << "[Elevator] Initialized elevator ID " << config.id << "\n";
}

void Elevator::run() {
    std::cout << "[Elevator] Running elevator logic...\n";
    while (true) {
        can_interface.receiveCommand();
        can_interface.sendStatus(current_floor);
        // 시뮬레이션용 floor 변경 로직
        current_floor = (current_floor + 1) % 20;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}