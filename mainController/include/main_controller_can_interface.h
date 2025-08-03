#pragma once

#include "manifest.h"  // 메인 컨트롤러 설정 정보

class MainControllerCANInterface {
public:
    explicit MainControllerCANInterface(const ControllerConfig& config);
    
    // 엘리베이터 위치 정보 전송
    void sendElevatorStatus(int elevator_id, int floor);
    
    // 버튼 누름 이벤트 수신 (floor panel에서 보냄)
    void receiveButtonPress();

    void sendEvInitialize();

    int receiveEvInitialize();

    void sendPanelInitialize();

    int receivePanelInitialize();

    void sendCANMessage(int p1, int p2);

private:
    int socket_fd;
    std::string can_interface; // CAN 인터페이스 이름 (예: "vcan0")

    // 내부 헬퍼 함수 - 소켓 초기화, 바인딩 등
    bool initSocket();
    bool receiveFrame(struct can_frame& frame, int timeout_ms);
};
