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

private:
    int socket_fd;
    int tx_id_base;  // 엘리베이터 상태 송신용 CAN ID 베이스 (elevator_id별 offset 사용 가능)
    int rx_id;       // 버튼 이벤트 수신용 CAN ID

    // 내부 헬퍼 함수 - 소켓 초기화, 바인딩 등
    bool initSocket(const char* interface_name);
};
