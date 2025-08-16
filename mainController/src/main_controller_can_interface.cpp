#include "main_controller_can_interface.h"

#include <cstring>
#include <iostream>
#if defined(__linux__)
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

constexpr int default_timeout_ms = 100;  // initialization 응답 확인용 timeout

MainControllerCANInterface::MainControllerCANInterface(const ControllerConfig& config)
    : can_interface(config.can_interface), socket_fd(-1)
{
#if defined(__linux__)
    if (!initSocket())
    {
        std::cerr << "Failed to initialize CAN socket on " << can_interface << "\n";
    }
#endif
}

MainControllerCANInterface::~MainControllerCANInterface()
{
#if defined(__linux__)
    if (socket_fd >= 0)
    {
        close(socket_fd);
    }
#endif
}

bool MainControllerCANInterface::initSocket()
{
#if defined(__linux__)
    socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (socket_fd < 0)
    {
        perror("socket");
        return false;
    }

    struct ifreq ifr;
    std::strncpy(ifr.ifr_name, can_interface.c_str(), IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = 0;

    if (ioctl(socket_fd, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl");
        return false;
    }

    struct sockaddr_can addr{};
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        return false;
    }
#endif
    std::cout << "[MAIN] Initialized socket on interface " << can_interface << "\n";
    return true;
}

bool MainControllerCANInterface::initializeElevator(int& elevator_id)
{
#if defined(__linux__)
    struct can_frame frame{};
    frame.can_id = 0x000 + elevator_id;
    frame.can_dlc = 1;
    frame.data[0] = 0xFF;  // Initialization command

    if (write(socket_fd, &frame, sizeof(frame)) < 0)
    {
        perror("write");
        return false;
    }

    // ----- 직접 select + read로 응답 대기 -----
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout{};
    timeout.tv_sec = default_timeout_ms / 1000;
    timeout.tv_usec = (default_timeout_ms % 1000) * 1000;

    if (select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout) > 0)
    {
        struct can_frame response_frame{};
        if (read(socket_fd, &response_frame, sizeof(response_frame)) > 0)
        {
            if ((response_frame.can_id & 0xF00) == 0x000 &&
                response_frame.data[0] == 0xFF)
            {
                elevator_id = response_frame.can_id - 0x000;
                std::cout << "[MAIN] Elevator " << elevator_id << " initialized successfully.\n";
                return true;
            }
        }
    }
#endif
    std::cerr << "[MAIN] Elevator " << elevator_id << " failed to initialize.\n";
    return false;
}

bool MainControllerCANInterface::initializePanel(int elevator_id, int floor)
{
#if defined(__linux__)
    struct can_frame frame{};
    frame.can_id = 0x100 + elevator_id;
    frame.can_dlc = 2;
    frame.data[0] = 0xFF;  // Initialization command
    frame.data[1] = static_cast<uint8_t>(floor);

    if (write(socket_fd, &frame, sizeof(frame)) < 0)
    {
        perror("write");
        return false;
    }

    // ----- 직접 select + read로 응답 대기 -----
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout{};
    timeout.tv_sec = default_timeout_ms / 1000;
    timeout.tv_usec = (default_timeout_ms % 1000) * 1000;

    if (select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout) > 0)
    {
        struct can_frame response_frame{};
        if (read(socket_fd, &response_frame, sizeof(response_frame)) > 0)
        {
            if (response_frame.can_id == (0x100 + elevator_id) &&
                response_frame.data[0] == 0xFF &&
                response_frame.data[1] == floor)
            {
                std::cout << "[MAIN] Panel for elevator " << elevator_id
                          << " at floor " << floor << " initialized successfully.\n";
                return true;
            }
        }
    }
#endif
    std::cerr << "[MAIN] Panel for elevator " << elevator_id
              << " at floor " << floor << " failed to initialize.\n";
    return false;
}

void MainControllerCANInterface::sendMoveCommand(int elevator_id, int floor)
{
    sendCommand(elevator_id, {static_cast<uint8_t>(floor)});
}

void MainControllerCANInterface::sendElevatorCommand(int elevator_id, int command)
{
    sendCommand(elevator_id, {static_cast<uint8_t>(command)});
}

void MainControllerCANInterface::sendCommand(int elevator_id, const std::vector<uint8_t>& data)
{
#if defined(__linux__)
    struct can_frame frame{};
    frame.can_id = static_cast<canid_t>(0x000 + elevator_id);
    frame.can_dlc = static_cast<__u8>(data.size());

    for (size_t i = 0; i < data.size() && i < sizeof(frame.data); ++i)
    {
        frame.data[i] = data[i];
    }

    if (write(socket_fd, &frame, sizeof(frame)) < 0)
    {
        perror("write");
    }
    else
    {
        std::cout << "[MAIN] Sent command to elevator " << elevator_id << " with data:";
        for (auto byte : data)
        {
            std::cout << " 0x" << std::hex << static_cast<int>(byte);
        }
        std::cout << std::dec << "\n";
    }
#endif
}

bool MainControllerCANInterface::receiveCommand()
{
#if defined(__linux__)
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);

    struct timeval timeout{};
    timeout.tv_sec = 0;  // non-blocking
    timeout.tv_usec = 0;

    if (select(socket_fd + 1, &read_fds, nullptr, nullptr, &timeout) > 0)
    {
        struct can_frame frame{};
        if (read(socket_fd, &frame, sizeof(frame)) > 0)
        {
            // 패널 버튼 메시지
            if (frame.can_id >= 0x100 && frame.can_id <= 0x1FF)
            {
                int elevator_id = frame.can_id - 0x100;
                int floor = frame.data[0];
                bool is_up_button = (frame.data[1] == 0x01);
                std::cout << "[MAIN] Received button press from panel for elevator "
                          << elevator_id << " at floor " << floor
                          << " (" << (is_up_button ? "UP" : "DOWN") << ")\n";

                panelRequest = {floor, is_up_button};
                updatePanelRequest = true;
            }
            // 엘리베이터 상태 메시지
            else if (frame.can_id >= 0x000 && frame.can_id <= 0x0FF)
            {
                int elevator_id = frame.can_id;
                int current_floor = frame.data[0];
                int elevator_status = frame.data[1];
                int direction = frame.data[2];
                int door = frame.data[3];
                std::cout << "[MAIN] Received status from elevator " << elevator_id
                          << ": currently at floor " << current_floor
                          << ": elevator direction " << direction << "\n";

                evMap->at(elevator_id).currentFloor = current_floor;
                evMap->at(elevator_id).direction = direction;
            }
            return true;
        }
    }
    else if (errno != 0)
    {
        perror("select");
    }
#endif
    return false;
}
