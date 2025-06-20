#pragma once
#include "panel_config.h"
#if 0
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
    void sendButtonPress(bool up);
    void receiveElevatorStatus();  // blocking or non-blocking

private:
    int socket_fd;
    int tx_id;
    int rx_id;
};

