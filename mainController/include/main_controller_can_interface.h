#pragma once

#include "manifest.h"  // 메인 컨트롤러 설정 정보

class MainControllerCANInterface {
public:
    explicit MainControllerCANInterface(const ControllerConfig& config);
    
    // Initialization
    bool initializeElevator(int elevator_id);
    bool initializePanel(int elevator_id, int floor);

    // Commands
    void sendMoveCommand(int elevator_id, int floor);
    void sendElevatorCommand(int elevator_id, int command); // e.g., open/close door

    // Receiving
    void receiveMessages();

private:
    int socket_fd;
    std::string can_interface;

    bool initSocket();
    bool receiveFrame(struct can_frame& frame, int timeout_ms);
    void processIncomingFrame(const struct can_frame& frame);
};
