#include "main_controller_can_interface.h"
#include <iostream>
#include <cstring>
#if 0
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/socket.h>
#endif

MainControllerCANInterface::MainControllerCANInterface(const ControllerConfig& config)
    : tx_id_base(config.can_tx_id_base), rx_id(config.can_rx_id), socket_fd(-1)
{
#if 0
    if (!initSocket("can0")) {  // 예: can0 인터페이스 사용
        std::cerr << "Failed to initialize CAN socket\n";
    }
#endif
}

bool MainControllerCANInterface::initSocket(const char* interface_name)
{
#if 0
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
    std::cout << "[CAN] Initialized socket on interface " << interface_name << "\n";
    return true;
}

void MainControllerCANInterface::sendElevatorStatus(int elevator_id, int floor)
{
#if 0
    struct can_frame frame {};
    frame.can_id = tx_id_base + elevator_id;  // 엘리베이터 별로 ID 분리
    frame.can_dlc = 1;
    frame.data[0] = static_cast<uint8_t>(floor);

    int nbytes = write(socket_fd, &frame, sizeof(frame));
    if (nbytes < 0) {
        perror("write");
    } else {
        std::cout << "[CAN] Sent elevator " << elevator_id << " floor status: " << floor << "\n";
    }
#endif
}

void MainControllerCANInterface::receiveButtonPress()
{
#if 0
    struct can_frame frame;
    int nbytes = read(socket_fd, &frame, sizeof(frame));
    if (nbytes > 0 && frame.can_id == rx_id) {
        bool up = frame.data[0] != 0;
        std::cout << "[CAN] Received button press: " << (up ? "UP" : "DOWN") << "\n";
        // 여기서 이벤트 처리 (스케줄러 호출 등)
    } else if (nbytes < 0) {
        perror("read");
    }
#endif
}
