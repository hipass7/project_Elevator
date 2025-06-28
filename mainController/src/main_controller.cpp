#include "main_controller.h"
#include <iostream>
#include <thread>
#include <chrono>

MainController::MainController(const ControllerConfig& config)
    : buildingName(config.building_name),
      numFloors(config.num_floors),
      numElevators(config.num_elevators),
      canRxId(config.can_rx_id),
      canTxIdBase(config.can_tx_id_base),
      scanIntervalMs(config.floor_panel_scan_interval_ms),
      canInterface(config) {

    std::cout << "[MainController] Initialized with " << numElevators
              << " elevators for building: " << buildingName << "\n";
}

void MainController::printConfigSummary() const {
    std::cout << "[Config] Floors: " << numFloors
              << ", Elevators: " << numElevators
              << ", CAN RX: 0x" << std::hex << canRxId
              << ", CAN TX base: 0x" << std::hex << canTxIdBase << std::dec
              << ", Panel Scan Interval: " << scanIntervalMs << " ms\n";
}

// 시스템 초기화 (예: 엘리베이터 및 층 패널 초기화)
void MainController::initialize() {
    std::cout << "[mainController] Initialization started." << std::endl;

    std::cout << "[MainController] System initializing...\n";

    // tx_id로 CAN 통신 통해서 연결된 Elevator들에게 초기화 요청을 보냄
    // ex. data[0] = 0xFF
    // Elevator들은 위 신호를 받으면 0xFF 0x01(id) 라는 식으로 return을 보냄
    // 그 데이터를 여기서 받아서 evMap에 추가

    // 엘리베이터 초기화 (evMap)
    canInterface.sendEvInitialize();
    for (int i = 0; i < numElevators; i++)
    {
        int evId = canInterface.receiveEvInitialize();
        if (evId == -1)
        {
            std::cout << "Invalid elevator id" << std::endl; // 오류 출력, 재부팅 로직 타야함
        }
        evMap[evId] = std::vector<int>{};
    }

    // panel 초기화 (panelList)
    canInterface.sendPanelInitialize();
    for (int i = 0; i < numFloors; i++)
    {
        int panelId = canInterface.receivePanelInitialize();
        panelList.emplace_back(panelId);
    }
    if (panelList.size() != numFloors)
    {
        std::cout << "Invalid panel id" << std::endl; // 오류 출력, 재부팅 로직 타야함
    }

    std::cout << "[mainController] Initialization completed." << std::endl;
}

// 시스템 메인 루프 실행
void MainController::run() {
    std::cout << "[mainController] System running..." << std::endl;

    while (true) {
        canInterface.receiveButtonPress();
        std::this_thread::sleep_for(std::chrono::milliseconds(scanIntervalMs));
    }

    std::cout << "[mainController] System stopped." << std::endl;
}
