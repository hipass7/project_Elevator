#include "main_controller.h"
#include <iostream>

MainController::MainController(const ControllerConfig& config)
    : buildingName(config.building_name),
      numFloors(config.num_floors),
      numElevators(config.num_elevators),
      canRxId(config.can_rx_id),
      canTxIdBase(config.can_tx_id_base),
      scanIntervalMs(config.floor_panel_scan_interval_ms) {
    
    for (const auto& elevConfig : config.elevators) {
        elevators.push_back(std::make_shared<Elevator>(elevConfig));
    }

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
    for (const auto& elevator : elevators) {
        elevator->printStatus();
    }

    // 파일을 불러옴 (ev들의 정보가 있는 json 파일 (list임))
    //em.initialize();

    std::cout << "[mainController] Initialization completed." << std::endl;
}

// 시스템 메인 루프 실행
void MainController::run() {
    std::cout << "[mainController] System running..." << std::endl;

    // 예시로 단순 루프를 사용
    // for (int tick = 0; tick < 10; ++tick) {
    //     std::cout << "Tick " << tick << std::endl;

    //     scheduler.step();  // 스케줄러에 명령 전달
    //     for (auto& elevator : elevators) {
    //         elevator->step();  // 각 엘리베이터 한 번 동작
    //     }
    // }

    std::cout << "[mainController] System stopped." << std::endl;
}
