#pragma once
#include "panel_config.h"
#if defined(__linux__)
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>
#include <cstring>
#endif

class PanelCANInterface {
public:
    explicit PanelCANInterface(const PanelConfig& config);
    ~PanelCANInterface();
    void sendButtonPress(bool up);
    void receiveElevatorStatus();

private:
    bool initSocket();

    int socket_fd;
    int tx_id;
    int rx_id;
    std::string can_interface;
};

