#include "elevator_can_interface.h"
#include <iostream>
#include <cstring>
#if 0
#include <unistd.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/ioctl.h>
#endif

ElevatorCANInterface::ElevatorCANInterface(const ElevatorConfig& config)
    : tx_id(config.can_tx_id), rx_id(config.can_rx_id), socket_fd(-1) {
#if 0
    if (!initSocket("vcan0")) {  // or "can0" for real hardware
        std::cerr << "[Elevator CAN] Failed to initialize SocketCAN on vcan0\n";
    } else {
        std::cout << "[Elevator CAN] Initialized with tx_id=0x" << std::hex << tx_id
                  << " rx_id=0x" << rx_id << std::dec << "\n";
    }
#endif
}

ElevatorCANInterface::~ElevatorCANInterface() {
#if 0
    if (socket_fd >= 0) {
        close(socket_fd);
    }
#endif
}

bool ElevatorCANInterface::initSocket(const char* interface_name) {
#if 0
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("socket");
        return false;
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, interface_name, IFNAMSIZ - 1);
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

void ElevatorCANInterface::sendElevatorStatus(int floor) {
#if 0
    struct can_frame frame {};
    frame.can_id = tx_id;
    frame.can_dlc = 1;
    frame.data[0] = static_cast<uint8_t>(floor);

    int nbytes = write(socket_fd, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("write");
    } else {
        std::cout << "[Elevator CAN] Sent current floor: " << floor << " (tx_id=0x"
                  << std::hex << tx_id << std::dec << ")\n";
    }
#endif
}

bool ElevatorCANInterface::receiveControlCommand() {
    auto openDoor = true;
#if 0
    struct can_frame frame;
    int nbytes = read(socket_fd, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("read");
        return false;
    }

    if (frame.can_id != rx_id || frame.can_dlc < 1) {
        return false;
    }

    // 예: 0 = 아무것도 안함, 1 = 문 열기
    openDoor = (frame.data[0] == 1);

    std::cout << "[Elevator CAN] Received command from controller (rx_id=0x"
              << std::hex << rx_id << std::dec << "): " << (openDoor ? "Open Door" : "No Action") << "\n";
#endif
    return openDoor;
}
