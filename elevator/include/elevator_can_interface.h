#pragma once

#include "manifest.h"

class ElevatorCANInterface {
public:
    explicit ElevatorCANInterface(const ElevatorConfig& config);
    ~ElevatorCANInterface();

    void sendElevatorStatus(int floor);   // 엘리베이터 현재 층 송신
    bool receiveControlCommand(bool& openDoor);  // 제어 명령 수신 (예: 문 열림 등)

private:
    bool initSocket(const char* interface_name);

    int socket_fd;
    int tx_id;
    int rx_id;
};
