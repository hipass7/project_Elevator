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
    void receiveCommand();

private:
    bool initSocket();

    int socket_fd;
    int floor;
    int elevator_id;
    std::string can_interface;
};

