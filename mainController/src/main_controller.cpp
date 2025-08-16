#include "main_controller.h"

#include <chrono>
#include <iostream>
#include <thread>

MainController::MainController(const std::string& config_path)
    : config(ControllerConfig::loadFromFile(config_path)),
      buildingName(config.building_name),
      numFloors(config.num_floors),
      numElevators(config.num_elevators),
      scanIntervalMs(config.floor_panel_scan_interval_ms),
      canInterface(config)
{
    std::cout << "[MainController] Initialized with " << numElevators
              << " elevators for building: " << buildingName << "\n";
}

void MainController::printConfigSummary() const
{
    std::cout << "[Config] Floors: " << numFloors
              << ", Elevators: " << numElevators
              << ", CAN RX: 0x" << std::hex << canRxId
              << ", CAN TX base: 0x" << std::hex << canTxIdBase << std::dec
              << ", Panel Scan Interval: " << scanIntervalMs << " ms\n";
}

// 시스템 초기화 (예: 엘리베이터 및 층 패널 초기화)
void MainController::initialize()
{
    std::cout << "[mainController] Initialization started." << std::endl;

    // canInterface.sendEvInitialize();
    for (int i = 0; i < numElevators; ++i)
    {
        int evId = -1;
        int retries = 5;
        while (retries-- > 0 && evId == -1)
        {
            (void)canInterface.initializeElevator(evId);
            if (evId == -1)
            {
                std::cerr << "Elevator initialization failed. Retrying..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        if (evId == -1)
        {
            // throw std::runtime_error("Failed to initialize all elevators.");
        }
        evMap[evId] = {};
    }

    // canInterface.sendPanelInitialize();
    for (int i = 0; i < numFloors; ++i)
    {
        int panelId = -1;
        int retries = 0;
        while (retries-- > 0 && panelId == -1)
        {
            // panelId = canInterface.receivePanelInitialize();
            if (panelId == -1)
            {
                std::cerr << "Panel initialization failed. Retrying..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }
        if (panelId == -1)
        {
            // throw std::runtime_error("Failed to initialize all panels.");
        }
        panelList.push_back(panelId);
    }
    canInterface.setEvMap(&evMap);
    std::cout << "[mainController] Initialization completed." << std::endl;
}

// 시스템 메인 루프 실행
void MainController::run()
{
    std::cout << "[mainController] System running..." << std::endl;
    int temp = 0;
    while (true)
    {
        canInterface.receiveCommand();  // 이제 이 함수는 논블로킹입니다.
        if (canInterface.updatePanelRequest)
        {
            canInterface.updatePanelRequest = false;
            panelRequest = canInterface.panelRequest;
            scheduleElevators();
        }
        // 3. 각 EV에 다음 목적지 하나만 보내기
        for (auto& [evId, evState] : evMap)
        {
            for (int t : evState.targets)
            {
                std::cout << t << " ";
            }
            // std::cout << " #" << evId << std::endl;
            if (!evState.targets.empty())
            {
                if ((evState.currentFloor == evState.targets[0]) && evState.direction == 1)
                {
                    evState.targets.erase(evState.targets.begin());
                    if (!evState.targets.empty())
                    {
                        canInterface.sendMoveCommand(evId, evState.targets[0]);
                        std::cout << "[MAIN] Elevator " << evId
                                  << " moving to next target floor " << evState.targets[0] << "\n";
                    }
                    else
                    {
                        std::cout << "[MAIN] Elevator " << evId
                                  << " has no more targets, going idle.\n";
                        canInterface.sendMoveCommand(evId, 0);
                    }
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(scanIntervalMs));
    }

    std::cout << "[mainController] System stopped." << std::endl;
}

void MainController::scheduleElevators()
{
    // 외부 요청 처리

    int reqFloor = panelRequest.first;
    bool reqUp = panelRequest.second;

    // 1. 가장 적합한 엘리베이터 찾기
    int bestEv = -1;
    int minDistance = INT_MAX;

    for (auto& [evId, evState] : evMap)
    {
        int distance = INT_MAX;

        if (evState.direction == 1)
        {
            // idle이면 단순 거리
            distance = abs(evState.currentFloor - reqFloor);
        }
        else if (evState.direction == 2 && reqFloor >= evState.currentFloor)
        {
            distance = reqFloor - evState.currentFloor;  // 올라가는 중 요청이 위에 있으면 OK
        }
        else if (evState.direction == 0 && reqFloor <= evState.currentFloor)
        {
            distance = evState.currentFloor - reqFloor;  // 내려가는 중 요청이 아래에 있으면 OK
        }

        if (distance < minDistance)
        {
            minDistance = distance;
            bestEv = evId;
        }
    }

    if (bestEv != -1)
    {
        ElevatorState& ev = evMap[bestEv];

        int dest = ev.targets.empty() ? -1 : ev.targets[0];
        // 2. targets에 올바른 위치에 삽입
        if (ev.direction >= 1)
        {  // 올라가는 중 or idle
            ev.targets.push_back(reqFloor);
            std::sort(ev.targets.begin(), ev.targets.end());
        }
        else
        {  // 내려가는 중
            ev.targets.push_back(reqFloor);
            std::sort(ev.targets.rbegin(), ev.targets.rend());
        }

        if (dest != ev.targets[0])
        {
            canInterface.sendMoveCommand(bestEv, ev.targets[0]);
            std::cout << "[MAIN] Scheduled elevator " << bestEv
                      << " to move to floor " << ev.targets[0] << "\n";
        }
        else
        {
            std::cout << "[MAIN] Elevator " << bestEv
                      << " is already at the target floor " << dest << "\n";
        }
    }
}
