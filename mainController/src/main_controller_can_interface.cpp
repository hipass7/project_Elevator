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

constexpr int timeout_ms = 300;

MainControllerCANInterface::MainControllerCANInterface(const ControllerConfig& config)
    : tx_id_base(config.can_tx_id_base), rx_id(config.can_rx_id), socket_fd(-1)
{
#if defined(__linux__)
    if (!initSocket("vcan0")) {
        std::cerr << "Failed to initialize CAN socket\n";
    }
#endif
}

bool MainControllerCANInterface::initSocket(const char* interface_name)
{
#if defined(__linux__)
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0) {
        perror("socket");
        return false;
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, interface_name, IFNAMSIZ-1);
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
    std::cout << "[MAIN] Initialized socket on interface " << interface_name << "\n";
    return true;
}

void MainControllerCANInterface::sendElevatorStatus(int elevator_id, int floor)
{
#if defined(__linux__)
    struct can_frame frame {};
    frame.can_id = tx_id_base + elevator_id;
    frame.can_dlc = 1;
    frame.data[0] = static_cast<uint8_t>(floor);

    int nbytes = write(socket_fd, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("write");
    } else {
        std::cout << "[MAIN] Sent elevator " << elevator_id << " floor status: " << floor << "\n";
    }
#endif
}

void MainControllerCANInterface::receiveButtonPress()
{
#if defined(__linux__)
    struct can_frame frame;

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ret > 0 && FD_ISSET(socket_fd, &read_fds)) {
        int nbytes = read(socket_fd, &frame, sizeof(frame));
        if (nbytes > 0 && frame.can_id == rx_id) {
            bool up = frame.data[0] != 0;
            std::cout << "[MAIN] Received button press: " << (up ? "UP" : "DOWN") << "\n";
        }
    } else if (ret == 0) {
        std::cout << "[MAIN] receiveButtonPress timeout\n";
    } else {
        perror("select");
    }
#endif
}

void MainControllerCANInterface::sendEvInitialize() {
#if defined(__linux__)
    struct can_frame frame{};
    frame.can_id = tx_id_base;
    frame.can_dlc = 1;
    frame.data[0] = 0xFF;

    int nbytes = write(socket_fd, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("[CAN] sendEvInitialize failed");
    } else {
        std::cout << "[CAN] Sent EV init command\n";
    }
#endif
}

int MainControllerCANInterface::receiveEvInitialize() {
#if defined(__linux__)
    struct can_frame frame;

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ret > 0 && FD_ISSET(socket_fd, &read_fds)) {
        int nbytes = read(socket_fd, &frame, sizeof(frame));
        if (nbytes > 0 && frame.data[0] == 0xFF) {
            std::cout << "[CAN] Received EV init response from EV ID: " << static_cast<int>(frame.data[1]) << "\n";
            return static_cast<int>(frame.data[1]);
        }
    } else if (ret == 0) {
        std::cout << "[CAN] receiveEvInitialize timeout\n";
    } else {
        perror("select");
    }
#endif
    return -1;
}

void MainControllerCANInterface::sendPanelInitialize() {
#if defined(__linux__)
    struct can_frame frame{};
    frame.can_id = tx_id_base;
    frame.can_dlc = 1;
    frame.data[0] = 0xFF;

    int nbytes = write(socket_fd, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("[CAN] sendPanelInitialize failed");
    } else {
        std::cout << "[CAN] Sent Panel init command\n";
    }
#endif
}

int MainControllerCANInterface::receivePanelInitialize() {
#if defined(__linux__)
    struct can_frame frame;

    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout;
    timeout.tv_sec = timeout_ms / 1000;
    timeout.tv_usec = (timeout_ms % 1000) * 1000;

    int ret = select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout);
    if (ret > 0 && FD_ISSET(socket_fd, &read_fds)) {
        int nbytes = read(socket_fd, &frame, sizeof(frame));
        if (nbytes > 0 && frame.data[0] == 0xFF) {
            std::cout << "[CAN] Received Panel init response from Floor: " << static_cast<int>(frame.data[1]) << "\n";
            return static_cast<int>(frame.data[1]);
        }
    } else if (ret == 0) {
        std::cout << "[CAN] receivePanelInitialize timeout\n";
    } else {
        perror("select");
    }
#endif
    return -1;
}
