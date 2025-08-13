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

constexpr int default_timeout_ms = 100; // Use a shorter timeout for initialization checks

MainControllerCANInterface::MainControllerCANInterface(const ControllerConfig& config)
    : can_interface(config.can_interface), socket_fd(-1)
{
#if defined(__linux__)
    if (!initSocket()) {
        std::cerr << "Failed to initialize CAN socket on " << can_interface << "\n";
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
    std::cout << "[MAIN] Initialized socket on interface " << can_interface << "\n";
    return true;
}

bool MainControllerCANInterface::initializeElevator(int& elevator_id) {
#if defined(__linux__)
    struct can_frame frame {};
    frame.can_id = 0x000 + elevator_id;
    frame.can_dlc = 1;
    frame.data[0] = 0xFF; // Initialization command

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("write");
        return false;
    }
    
    // Wait for response
    struct can_frame response_frame;
    if (receiveFrame(response_frame, default_timeout_ms)) {
        // 비트마스크로 상위 비트를 제거하고 확인 (0x000 ~ 0x0FF 범위만 허용)
        if ( (response_frame.can_id & 0xF00) == 0x000 ) {
            int received_id = response_frame.can_id - 0x000;
            if (response_frame.data[0] == 0xFF) {
                elevator_id = received_id; // parameter에 반환
                std::cout << "[MAIN] Elevator " << elevator_id << " initialized successfully.\n";
                return true;
            }
        } else {
            std::cerr << "[MAIN] Ignored CAN ID: 0x" << std::hex 
                      << response_frame.can_id << std::dec << "\n";
        }
    }
#endif
    std::cerr << "[MAIN] Elevator " << elevator_id << " failed to initialize.\n";
    return false;
}

bool MainControllerCANInterface::initializePanel(int elevator_id, int floor) {
#if defined(__linux__)
    struct can_frame frame {};
    // Panels are addressed by the elevator they serve
    frame.can_id = 0x100 + elevator_id; 
    frame.can_dlc = 2;
    frame.data[0] = 0xFF; // Initialization command
    frame.data[1] = static_cast<uint8_t>(floor);

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("write");
        return false;
    }

    // Wait for response
    struct can_frame response_frame;
    if (receiveFrame(response_frame, default_timeout_ms)) {
        if (response_frame.can_id == (0x100 + elevator_id) && response_frame.data[0] == 0xFF && response_frame.data[1] == floor) {
            std::cout << "[MAIN] Panel for elevator " << elevator_id << " at floor " << floor << " initialized successfully.\n";
            return true;
        }
    }
#endif
    std::cerr << "[MAIN] Panel for elevator " << elevator_id << " at floor " << floor << " failed to initialize.\n";
    return false;
}

void MainControllerCANInterface::sendMoveCommand(int elevator_id, int floor) {
    sendCommand(elevator_id, { static_cast<uint8_t>(floor) });
}

void MainControllerCANInterface::sendElevatorCommand(int elevator_id, int command) {
    sendCommand(elevator_id, { static_cast<uint8_t>(command) });
}

void MainControllerCANInterface::sendCommand(int elevator_id, const std::vector<uint8_t>& data) {
#if defined(__linux__)
    struct can_frame frame {};
    frame.can_id = static_cast<canid_t>(0x000 + elevator_id);
    frame.can_dlc = static_cast<__u8>(data.size());

    for (size_t i = 0; i < data.size() && i < sizeof(frame.data); ++i) {
        frame.data[i] = data[i];
    }

    if (write(socket_fd, &frame, sizeof(frame)) < 0) {
        perror("write");
    } else {
        std::cout << "[MAIN] Sent command to elevator " << elevator_id << " with data:";
        for (auto byte : data) {
            std::cout << " 0x" << std::hex << static_cast<int>(byte);
        }
        std::cout << std::dec << "\n";
    }
#endif
}

void MainControllerCANInterface::receiveMessages() {
    requests.clear();
    struct can_frame frame;
    // Use a longer timeout for general message receiving
    if (receiveFrame(frame, 0)) { 
        processIncomingFrame(frame);
    }
}

void MainControllerCANInterface::processIncomingFrame(const struct can_frame& frame) {
    // Check if it's a button press from a panel
    if (frame.can_id >= 0x100 && frame.can_id <= 0x1FF) {
        int elevator_id = frame.can_id - 0x100;
        int floor = frame.data[0];
        bool is_up_button = (frame.data[1] == 0x01);
        std::cout << "[MAIN] Received button press from panel for elevator " << elevator_id 
                  << " at floor " << floor << " (" << (is_up_button ? "UP" : "DOWN") << ")\n";
        requests.emplace_back(floor, is_up_button);
        // Here you would add the request to the elevator's queue
    }
    // Check if it's a status update from an elevator
    else if (frame.can_id >= 0x000 && frame.can_id <= 0x0FF) {
        int elevator_id = frame.can_id;
        int current_floor = frame.data[0];
        int elevator_status = frame.data[1];
        int direction = frame.data[2];
        std::cout << "[MAIN] Received status from elevator " << elevator_id 
                  << ": currently at floor " << current_floor 
                  << ": elevator status " << elevator_status << "\n";
        // Here you would update the elevator's state
        if (direction == 0 && requests.size() > 0) {
            requests.erase(requests.begin());
        }
    }
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
    }
    else if (ret < 0) {
        perror("select");
    }
#endif
    return false;
}