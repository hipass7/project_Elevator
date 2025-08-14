#pragma once

#include "manifest.h"  // 메인 컨트롤러 설정 정보
#include <map>

struct ElevatorState {
    int currentFloor = 1;
    int direction = 1; // 2: up, 0: down, 1: idle
    std::vector<int> targets;
};

class MainControllerCANInterface {
public:
    explicit MainControllerCANInterface(const ControllerConfig& config);
    
    // Initialization
    bool initializeElevator(int& elevator_id);
    bool initializePanel(int elevator_id, int floor);
    void setEvMap(std::map<int, ElevatorState>* map) { evMap = map; }

    // Commands
    void sendMoveCommand(int elevator_id, int floor);
    void sendElevatorCommand(int elevator_id, int command); // e.g., open/close door
    void sendCommand(int elevator_id, const std::vector<uint8_t>& data);

    // Receiving
    void receiveMessages();

    std::pair<int, bool> panelRequest;
    bool updatePanelRequest = false;

private:
    int socket_fd;
    std::string can_interface;
    std::map<int, ElevatorState>* evMap = nullptr;

    bool initSocket();
    bool receiveFrame(struct can_frame& frame, int timeout_ms);
    void processIncomingFrame(const struct can_frame& frame);
};
