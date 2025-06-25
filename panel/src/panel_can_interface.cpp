#include "panel_can_interface.h"
#include <iostream>

PanelCANInterface::PanelCANInterface(const PanelConfig& config)
    : tx_id(config.can_tx_id), rx_id(config.can_rx_id), socket_fd(-1) {
#if defined(__linux__)
    struct ifreq ifr;
    struct sockaddr_can addr;

    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("socket");
        return;
    }

    std::strcpy(ifr.ifr_name, "vcan0");
    ioctl(socket_fd, SIOCGIFINDEX, &ifr);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return;
    }
#endif
    std::cout << "[PANEL] Bound to vcan0, tx_id=" << tx_id << ", rx_id=" << rx_id << "\n";
}

void PanelCANInterface::sendButtonPress(bool up) {
#if defined(__linux__)
    struct can_frame frame;
    frame.can_id = tx_id;
    frame.can_dlc = 1;
    frame.data[0] = up ? 1 : 0;

    int nbytes = write(socket_fd, &frame, sizeof(struct can_frame));
    if (nbytes < 0) {
        perror("write");
    } else {
        std::cout << "[PANEL] Sent button press: " << (up ? "UP" : "DOWN") << "\n";
    }
#endif
}

void PanelCANInterface::receiveElevatorStatus() {
#if defined(__linux__)
    struct can_frame frame;
    int nbytes = read(socket_fd, &frame, sizeof(struct can_frame));
    if (nbytes > 0 && frame.can_id == rx_id) {
        std::cout << "[PANEL] Received elevator status from id=" << rx_id
                  << " floor=" << static_cast<int>(frame.data[0]) << "\n";
    }
#endif
}
