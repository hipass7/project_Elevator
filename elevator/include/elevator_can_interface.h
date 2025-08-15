#pragma once

#include "elevator_config.h"

enum class ElevatorState : uint8_t
{                     // data[1] 통해서 보낼 상태
    Emergency = 0,    // emergency state if true
    Status = 1,       // Current Floor
    Request = 2,      // Internal Request
    Initialize = 255  // Initialization Command
};

class Elevator;  // Forward declaration

class ElevatorCANInterface
{
   public:
    ElevatorCANInterface(Elevator& elevator, const ElevatorConfig& config);
    ~ElevatorCANInterface();

    bool sendCommand(const ElevatorState& state);  // 엘리베이터 현재 층 송신
    bool receiveCommand(int& dest);                // 제어 명령 수신 (예: 문 열림 등)

   private:
    bool initSocket();

    Elevator& elevatorRef;  // Reference to the Elevator object
    int socket_fd;
    int id;
    std::string can_interface;
};
