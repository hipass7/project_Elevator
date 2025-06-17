#pragma once
#include "panel_config.h"

class PanelCANInterface {
public:
    explicit PanelCANInterface(const PanelConfig& config);
    void sendButtonPress(bool up);
    void receiveElevatorStatus();  // blocking or non-blocking

private:
    int socket_fd;
    int tx_id;
    int rx_id;
};

