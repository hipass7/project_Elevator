#include "elevator.h"

#include <iostream>

// 생성자
Elevator::Elevator() {
    std::cout << "[Elevator] Constructor called." << std::endl;
    // 필요한 초기화가 있다면 여기에 작성
}

// 소멸자
Elevator::~Elevator() {
    std::cout << "[ElevatorManager] Destructor called." << std::endl;
}