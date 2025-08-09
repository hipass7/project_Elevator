#pragma once

#include "elevator_config.h"

enum class ElevatorState : uint8_t {
    emergency = 0,   // true면 비상 상황
    status = 1,   // 예: 0=Idle, 1=Moving, 2=Door Open ...
    request = 2  // 요청 층
};

class ElevatorCANInterface {
public:
    explicit ElevatorCANInterface(const ElevatorConfig& config);
    ~ElevatorCANInterface();

    void sendCommand(int floor, const ElevatorState& state);   // 엘리베이터 현재 층 송신
    bool receiveControlCommand();  // 제어 명령 수신 (예: 문 열림 등)

private:
    bool initSocket();

    int socket_fd;
    int id;
    std::string can_interface;
};
