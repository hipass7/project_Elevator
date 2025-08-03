#include "elevator.h"
#include <iostream>
#include <thread>
#include <chrono>

Elevator::Elevator(const std::string& config_path)
    : config(ElevatorConfig::loadFromFile(config_path)), 
      can_interface(config), 
      current_floor(0) {
    std::cout << "[Elevator] Initialized elevator ID " << config.id << "\n";
}

void Elevator::run() {
    std::cout << "[Elevator] Running elevator logic...\n";
    while (true) {
        if (can_interface.receiveControlCommand()) {
            // '문 열림' 명령 처리
            std::cout << "[Elevator] Door opening...\n";
            std::this_thread::sleep_for(std::chrono::seconds(config.door_open_duration_sec));
            std::cout << "[Elevator] Door closed.\n";
        }

        // 주기적으로 현재 층 상태 전송
        can_interface.sendElevatorStatus(current_floor);

        // TODO: 여기에 실제 엘리베이터 이동 및 상태 관리 로직 추가
        // (예: 목표 층으로 이동, 문 상태 관리 등)

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 짧은 대기
    }
}
