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
    : floor(config.floor), elevator_id(config.elevator_id), can_interface(config.can_interface), socket_fd(-1) {
#if defined(__linux__)
    if (!initSocket()) {
        std::cerr << "[PANEL CAN] Failed to initialize SocketCAN on " << can_interface << "\n";
    } else {
        std::cout << "[PANEL CAN] Initialized with floor=" << floor << ", elevator_id=" << elevator_id << "\n";
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
    frame.can_id = 0x100 + elevator_id;
    frame.can_dlc = 2;
    frame.data[0] = static_cast<uint8_t>(floor);
    frame.data[1] = up ? 0x01 : 0x00;

    if (write(socket_fd, &frame, sizeof(struct can_frame)) < 0) {
        perror("write");
    } else {
        std::cout << "[PANEL] Sent button press: " << (up ? "UP" : "DOWN") << " from floor " << floor << "\n";
    }
#endif
}

void PanelCANInterface::receiveCommand() {
#if defined(__linux__)
    struct can_frame frame;
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 1000; // 1ms timeout

    int ret = select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ret > 0 && FD_ISSET(socket_fd, &read_fds)) {
        if (read(socket_fd, &frame, sizeof(frame)) > 0) {
            // Check for commands addressed to the elevator this panel serves
            if (frame.can_id == (0x100 + elevator_id)) {
                // Initialization command
                if (frame.can_dlc == 2 && frame.data[0] == 0xFF && frame.data[1] == floor) {
                    std::cout << "[Panel " << floor << "] Received initialization command.\n";
                    struct can_frame response_frame {};
                    response_frame.can_id = 0x100 + elevator_id;
                    response_frame.can_dlc = 2;
                    response_frame.data[0] = 0xFF;
                    response_frame.data[1] = static_cast<uint8_t>(floor);
                    if (write(socket_fd, &response_frame, sizeof(response_frame)) < 0) {
                        perror("write response");
                    }
                }
            } 
            // Check for status updates from the elevator this panel serves
            else if (frame.can_id == elevator_id) {
                int current_floor = frame.data[0];
                std::cout << "[Panel " << floor << "] Received elevator status update. Current floor: " << current_floor << "\n";
                // Here, you would update the panel's display LEDs
            }
        }
    }
#endif
}
