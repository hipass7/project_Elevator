#include "elevator.h"

Elevator::Elevator(const ElevatorConfig& config)
    : id(config.id),
      doorOpenDurationSec(config.door_open_duration_sec),
      disabledButtons(config.disabled_buttons),
      canTxId(config.can_tx_id),
      canRxId(config.can_rx_id) {
    std::cout << "[Elevator] Initialized elevator #" << id << "\n";
}

void Elevator::printStatus() const {
    std::cout << "[Elevator] ID: " << id
              << ", Door Open Duration: " << doorOpenDurationSec << "s"
              << ", CAN TX: 0x" << std::hex << canTxId
              << ", CAN RX: 0x" << std::hex << canRxId << std::dec << "\n";
}
