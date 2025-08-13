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
        // 엘리베이터의 현재 위치, 방향(위/아래/정지), 상태(열림/닫힘/긴급) 확인 // 기존
        std::cout << "~~~" << std::endl;
        int dest = 0;

        // 1. dest 확인
        if (can_interface.receiveCommand(dest)) {
            dest_floor = dest;
        }

        // 2. 새로운 엘리베이터 위치 파악 (센서 input)
        int now_loc = checkCurrentFloor();

        // 3. 기존 위치와 새로 파악한 위치가 다를 경우, 상태 update
        bool update = false;
        if (now_loc != current_floor) {
            current_floor = now_loc;
            updateDirection();
            update = true;

            // 4. update면 send
            can_interface.sendCommand(current_floor, ElevatorState::status);
        }

        // 5. dest 도착인지 확인
        if (current_floor == dest_floor) {
            // 6. true면 진행
            stopAndOpenDoor();
        }

        // TODO: 여기에 실제 엘리베이터 이동 및 상태 관리 로직 추가
        // (예: 목표 층으로 이동, 문 상태 관리 등)

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 짧은 대기
    }
}

int Elevator::checkCurrentFloor() {
    // 센서로 확인 (지금은 임시)

    return current_floor + static_cast<int8_t>(direction);
}

void Elevator::updateDirection() {
        if (current_floor < dest_floor)
            direction = Direction::Up;
        else if (current_floor > dest_floor)
            direction = Direction::Down;
        else
            direction = Direction::Stop;
}

void Elevator::stopAndOpenDoor() {
    // receive는 열어두고 내부 input만 받고 아무 로직하지 않음 (문 열린 시간 동안)
    direction = Direction::Stop;
    doorOpen = true;
    can_interface.sendCommand(current_floor, ElevatorState::status);
    // '문 열림' 명령 처리
    std::cout << "[INFO] Arrived at floor " << current_floor << ". Door opening...\n";
    std::this_thread::sleep_for(std::chrono::seconds(config.door_open_duration_sec));
    std::cout << "[Elevator] Door closed.\n";
    
    doorOpen = false;
    // 실제 문 열기/정지 로직 삽입
}