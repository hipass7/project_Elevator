#include "elevator_can_interface.h"
#include <iostream>

ElevatorCANInterface::ElevatorCANInterface(const ElevatorConfig& config)
    : tx_id(config.can_tx_id), rx_id(config.can_rx_id), socket_fd(-1) {
    std::cout << "[CAN] Elevator CAN Interface ready. TX: " << tx_id << ", RX: " << rx_id << "\n";
}

void ElevatorCANInterface::sendStatus(int floor) {
    std::cout << "[CAN] Elevator send current floor: " << floor << " (TX: " << tx_id << ")\n";
}

void ElevatorCANInterface::receiveCommand() {
    std::cout << "[CAN] Elevator waiting for command (RX: " << rx_id << ")\n";
}
