#include "elevator_can_interface.h"
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

ElevatorCANInterface::ElevatorCANInterface(const ElevatorConfig& config)
    : id(config.id), can_interface(config.can_interface), socket_fd(-1) {
#if defined(__linux__)
    if (!initSocket()) {
        std::cerr << "[Elevator CAN] Failed to initialize SocketCAN on " << can_interface << "\n";
    } else {
        std::cout << "[Elevator CAN] Initialized with id=" << id << "\n";
    }
#endif
}

ElevatorCANInterface::~ElevatorCANInterface() {
#if defined(__linux__)
    if (socket_fd >= 0) {
        close(socket_fd);
    }
#endif
}

bool ElevatorCANInterface::initSocket() {
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

void ElevatorCANInterface::sendCommand(int floor, const ElevatorState& state, int direction) {
#if defined(__linux__)
    struct can_frame frame {};
    frame.can_id = 0x000 + id;
    frame.can_dlc = 3;
    frame.data[0] = static_cast<uint8_t>(floor);
    frame.data[1] = static_cast<uint8_t>(state);
    frame.data[2] = static_cast<uint8_t>(direction);
    if (state == ElevatorState::status) {
        // if dest - floor > 0 --> frame.data[2] 0x01
    }

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("write");
    } else {
        std::cout << "[Elevator CAN] Sent current floor: " << floor << " (tx_id=0x"
                  << std::hex << (0x000 + id) << std::dec << ")\n";
    }
#endif
}

void ElevatorCANInterface::sendCommand() { // simple_mode 전용
#if defined(__linux__)
        struct can_frame frame {};
        frame.can_id = 0x000 + id;
        frame.can_dlc = 1;
        frame.data[0] = 0xFF;

        if (write(socket_fd, &frame, sizeof(frame)) < 0) {
            perror("write");
        } else {
            std::cout << "[Elevator CAN] Sent simple command: 0xFF\n";
        }
#endif
}

bool ElevatorCANInterface::receiveCommand(int& dest) {
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
            if (frame.can_id == id) { // Addressed to this elevator
                // Initialization Command
                if (frame.can_dlc == 1 && frame.data[0] == 0xFF) {
                    std::cout << "[Elevator " << id << "] Received initialization command.\n";
                    struct can_frame response_frame {};
                    response_frame.can_id = id;
                    response_frame.can_dlc = 2;
                    response_frame.data[0] = 0xFF;
                    response_frame.data[1] = static_cast<uint8_t>(id);
                    if (write(socket_fd, &response_frame, sizeof(response_frame)) < 0) {
                        perror("write response");
                    }
                    return false; // Not a movement command
                } 
                // Move Command
                // else if (frame.can_dlc == 2 && frame.data[0] == 0x00) {
                //     int target_floor = frame.data[1];
                //     std::cout << "[Elevator " << id << "] Received move command to floor " << target_floor << "\n";
                //     // Here, you would trigger the elevator movement logic
                //     // For now, just acknowledge by sending status.
                //     // sendElevatorStatus(target_floor);
                //     return false; // Placeholder
                // } 
                // // Open Door Command
                // else if (frame.can_dlc == 1 && frame.data[0] == 1) {
                //      std::cout << "[Elevator " << id << "] Received open door command.\n";
                //      return true; // Indicates door should be opened
                // }
                // Receive destination command from main controller
                else if (frame.can_dlc == 1) {
                    dest = frame.data[0];
                    std::cout << "[Elevator " << id << "] Received : " << dest << " floor is destination.\n";
                    return true;
                }
            }
        }
    }
#endif
    return false;
}
