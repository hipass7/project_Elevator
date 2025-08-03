#include "panel_can_interface.h"
#include <iostream>
#include <cstring>
#if defined(__linux__)
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#endif

PanelCANInterface::PanelCANInterface(const PanelConfig& config)
    : tx_id(config.can_tx_id), rx_id(config.can_rx_id), can_interface(config.can_interface), socket_fd(-1) {
#if defined(__linux__)
    if (!initSocket()) {
        std::cerr << "[PANEL CAN] Failed to initialize SocketCAN on " << can_interface << "\n";
    } else {
        std::cout << "[PANEL CAN] Initialized with tx_id=" << tx_id << ", rx_id=" << rx_id << "\n";
    }
#endif
}

PanelCANInterface::~PanelCANInterface() {
#if defined(__linux__)
    if (socket_fd >= 0) {
        close(socket_fd);
    }
#endif
}

bool PanelCANInterface::initSocket() {
#if defined(__linux__)
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("socket");
        return false;
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, can_interface.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        return false;
    }

    struct sockaddr_can addr {};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return false;
    }
#endif
    return true;
}

void PanelCANInterface::sendButtonPress(bool up) {
#if defined(__linux__)
    struct can_frame frame;
    frame.can_id = tx_id;
    frame.can_dlc = 1;
    frame.data[0] = up ? 1 : 0;

    if (write(socket_fd, &frame, sizeof(struct can_frame)) < 0) {
        perror("write");
    } else {
        std::cout << "[PANEL] Sent button press: " << (up ? "UP" : "DOWN") << "\n";
    }
#endif
}

void PanelCANInterface::receiveElevatorStatus() {
#if defined(__linux__)
    struct can_frame frame;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000; // 1ms timeout for non-blocking behavior

    int ret = select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ret > 0 && FD_ISSET(socket_fd, &read_fds)) {
        if (read(socket_fd, &frame, sizeof(frame)) > 0 && frame.can_id == rx_id) {
            std::cout << "[PANEL] Received elevator status from id=" << rx_id
                      << " floor=" << static_cast<int>(frame.data[0]) << "\n";
        }
    }
#endif
}
