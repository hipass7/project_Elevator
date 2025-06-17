#include "panel_can_interface.h"
#include <iostream>

PanelCANInterface::PanelCANInterface(const PanelConfig& config)
    : tx_id(config.can_tx_id), rx_id(config.can_rx_id), socket_fd(-1) {
    // SocketCAN 초기화는 여기서 구현 가능
    std::cout << "[CAN] Init CAN interface with tx_id=" << tx_id << ", rx_id=" << rx_id << "\n";
}

void PanelCANInterface::sendButtonPress(bool up) {
    std::cout << "[CAN] Send " << (up ? "UP" : "DOWN") << " button press via CAN tx_id=" << tx_id << "\n";
    // 실제로는 CAN 프레임 구성해서 socket_fd로 전송
}

void PanelCANInterface::receiveElevatorStatus() {
    std::cout << "[CAN] Waiting for elevator status on rx_id=" << rx_id << "\n";
    // 실제로는 socket_fd에서 CAN 수신 로직
}
