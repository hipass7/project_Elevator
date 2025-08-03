#include "main_controller.h"
#include <iostream>
#include <thread>
#include <chrono>

MainController::MainController(const ControllerConfig& config)
    : buildingName(config.building_name),
      numFloors(config.num_floors),
      numElevators(config.num_elevators),
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

    canInterface.sendEvInitialize();
    for (int i = 0; i < numElevators; ++i) {
        int evId = -1;
        int retries = 3;
        while (retries-- > 0 && evId == -1) {
            evId = canInterface.receiveEvInitialize();
            if (evId == -1) {
                std::cerr << "Elevator initialization failed. Retrying..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        if (evId == -1) {
            throw std::runtime_error("Failed to initialize all elevators.");
        }
        evMap[evId] = {};
    }

    canInterface.sendPanelInitialize();
    for (int i = 0; i < numFloors; ++i) {
        int panelId = -1;
        int retries = 3;
        while (retries-- > 0 && panelId == -1) {
            panelId = canInterface.receivePanelInitialize();
            if (panelId == -1) {
                std::cerr << "Panel initialization failed. Retrying..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        if (panelId == -1) {
            throw std::runtime_error("Failed to initialize all panels.");
        }
        panelList.push_back(panelId);
    }

    std::cout << "[mainController] Initialization completed." << std::endl;
}

// 시스템 메인 루프 실행
void MainController::run() {
    std::cout << "[mainController] System running..." << std::endl;

    while (true) {
        canInterface.receiveButtonPress(); // 이제 이 함수는 논블로킹입니다.
        // 다른 주기적인 작업들을 여기에 추가할 수 있습니다.
        std::this_thread::sleep_for(std::chrono::milliseconds(scanIntervalMs));
    }

    std::cout << "[mainController] System stopped." << std::endl;
}
