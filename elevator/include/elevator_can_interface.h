#pragma once
#include "manifest.h"

class ElevatorCANInterface {
public:
    explicit ElevatorCANInterface(const ElevatorConfig& config);
    void sendStatus(int floor);
    void receiveCommand();

private:
    int tx_id;
    int rx_id;
    int socket_fd;
};