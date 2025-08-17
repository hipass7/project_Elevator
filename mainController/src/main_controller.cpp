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
    int reqFloor = panelRequest.first;
    bool reqUp = panelRequest.second;

    int bestEv = -1;
    int minCost = INT_MAX;

    // 1. 먼저 방향이 맞는 엘리베이터 탐색
    for (auto& [evId, evState] : evMap)
    {
        bool directionMatch = false;
        int distance = INT_MAX;

        if (evState.direction == 1) // idle
        {
            directionMatch = true;
            distance = abs(evState.currentFloor - reqFloor);
        }
        else if (evState.direction == 2 && reqUp && reqFloor >= evState.currentFloor)
        {
            // 올라가는 중 + 요청이 위에 있음
            directionMatch = true;
            distance = reqFloor - evState.currentFloor;
        }
        else if (evState.direction == 0 && !reqUp && reqFloor <= evState.currentFloor)
        {
            // 내려가는 중 + 요청이 아래에 있음
            directionMatch = true;
            distance = evState.currentFloor - reqFloor;
        }

        if (directionMatch && distance < minCost)
        {
            minCost = distance;
            bestEv = evId;
        }
    }

    // 2. 방향이 맞는 엘리베이터가 없다면, "모든 일정 끝낸 후" 기준으로 배정
    if (bestEv == -1)
    {
        for (auto& [evId, evState] : evMap)
        {
            int simulatedPos = evState.currentFloor;
            int totalDistance = 0;

            // 현재 targets를 끝까지 돌았을 때의 최종 위치 & 총 거리 추정
            for (int t : evState.targets)
            {
                totalDistance += abs(simulatedPos - t);
                simulatedPos = t;
            }

            // 마지막 target 이후 reqFloor까지 이동
            totalDistance += abs(simulatedPos - reqFloor);

            if (totalDistance < minCost)
            {
                minCost = totalDistance;
                bestEv = evId;
            }
        }
    }

    // 3. bestEv에 요청 할당
    if (bestEv != -1)
    {
        ElevatorState& ev = evMap[bestEv];
        int dest = ev.targets.empty() ? -1 : ev.targets[0];

        if (ev.direction == 1 || ev.direction == 2) // idle or up
        {
            ev.targets.push_back(reqFloor);
            std::sort(ev.targets.begin(), ev.targets.end());
        }
        else if (ev.direction == 0) // down
        {
            ev.targets.push_back(reqFloor);
            std::sort(ev.targets.rbegin(), ev.targets.rend());
        }
        else
        {
            // 방향 안 맞을 때 → 그냥 마지막에 붙인다
            ev.targets.push_back(reqFloor);
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

