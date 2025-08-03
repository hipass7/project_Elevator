#include "main_controller_can_interface.h"
#include <iostream>
#include <cstring>
#if defined(__linux__)
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#endif

constexpr int default_timeout_ms = 300;

MainControllerCANInterface::MainControllerCANInterface(const ControllerConfig& config)
    : tx_id_base(config.can_tx_id_base), 
      rx_id(config.can_rx_id), 
      can_interface(config.can_interface), 
      socket_fd(-1)
{
#if defined(__linux__)
    if (!initSocket()) {
        std::cerr << "Failed to initialize CAN socket on " << can_interface << "
";
    }
#endif
}

bool MainControllerCANInterface::initSocket()
{
#if defined(__linux__)
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("socket");
        return false;
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, can_interface.c_str(), IFNAMSIZ-1);
    ifr.ifr_name[IFNAMSIZ-1] = 0;

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
    std::cout << "[MAIN] Initialized socket on interface " << can_interface << "
";
    return true;
}

void MainControllerCANInterface::sendElevatorStatus(int elevator_id, int floor)
{
#if defined(__linux__)
    struct can_frame frame {};
    frame.can_id = tx_id_base + elevator_id;
    frame.can_dlc = 1;
    frame.data[0] = static_cast<uint8_t>(floor);

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("write");
    } else {
        std::cout << "[MAIN] Sent elevator " << elevator_id << " floor status: " << floor << "
";
    }
#endif
}

bool MainControllerCANInterface::receiveFrame(struct can_frame& frame, int timeout_ms) {
#if defined(__linux__)
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ret > 0 && FD_ISSET(socket_fd, &read_fds)) {
        if (read(socket_fd, &frame, sizeof(frame)) > 0) {
            return true;
        }
    } else if (ret == 0) {
        // Timeout
    } else {
        perror("select");
    }
#endif
    return false;
}

void MainControllerCANInterface::receiveButtonPress()
{
    struct can_frame frame;
    if (receiveFrame(frame, default_timeout_ms)) {
        if (frame.can_id == rx_id) {
            bool up = frame.data[1] != 0;
            std::cout << "[MAIN] Received button press: " << (up ? "UP" : "DOWN") << "
";
        }
    }
}

void MainControllerCANInterface::sendEvInitialize() {
#if defined(__linux__)
    struct can_frame frame{};
    frame.can_id = tx_id_base;
    frame.can_dlc = 1;
    frame.data[0] = 0xFF;

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("[CAN] sendEvInitialize failed");
    } else {
        std::cout << "[CAN] Sent EV init command
";
    }
#endif
}

int MainControllerCANInterface::receiveEvInitialize() {
    struct can_frame frame;
    if (receiveFrame(frame, default_timeout_ms) && frame.data[0] == 0xFF) {
        std::cout << "[CAN] Received EV init response from EV ID: " << static_cast<int>(frame.data[1]) << "
";
        return static_cast<int>(frame.data[1]);
    }
    return -1;
}

void MainControllerCANInterface::sendPanelInitialize() {
#if defined(__linux__)
    struct can_frame frame{};
    frame.can_id = tx_id_base;
    frame.can_dlc = 1;
    frame.data[0] = 0xFF;

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("[CAN] sendPanelInitialize failed");
    } else {
        std::cout << "[CAN] Sent Panel init command
";
    }
#endif
}

int MainControllerCANInterface::receivePanelInitialize() {
    struct can_frame frame;
    if (receiveFrame(frame, default_timeout_ms) && frame.data[0] == 0xFF) {
        std::cout << "[CAN] Received Panel init response from Floor: " << static_cast<int>(frame.data[1]) << "
";
        return static_cast<int>(frame.data[1]);
    }
    return -1;
}

void MainControllerCANInterface::sendCANMessage(int p1, int p2)
{
#if defined(__linux__)
    struct can_frame frame {};
    frame.can_id = 0x200 + p1;
    frame.can_dlc = 1;
    frame.data[0] = static_cast<uint8_t>(p2);

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("write");
    } else {
        std::cout << "[MAIN] Sent CAN message with ID 0x" << std::hex << frame.can_id << " and data " << std::dec << p2 << "
";
    }
#endif
}
