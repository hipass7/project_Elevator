#pragma once

#include "elevator_config.h"

class ElevatorCANInterface {
public:
    explicit ElevatorCANInterface(const ElevatorConfig& config);
    ~ElevatorCANInterface();

    void sendElevatorStatus(int floor);   // 엘리베이터 현재 층 송신
    void sendFloorRequestButton(int floor);
    bool receiveControlCommand();  // 제어 명령 수신 (예: 문 열림 등)

private:
    bool initSocket();

    int socket_fd;
    int id;
    std::string can_interface;
};
