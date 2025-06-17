#pragma once

#include <vector>
#include <memory>
#include "elevatorManager.h"
class Scheduler; // #include "scheduler.h"
class FloorPanel; // #include "floorPanel.h"

class BuildingSystem {
public:
    // 기본 생성자
    BuildingSystem();

    // 소멸자
    ~BuildingSystem();

    void initialize();

    void run();

    // 엘리베이터 추가
    //void addElevator(std::shared_ptr<Elevator> elevator);

    // 스케줄러 접근자
    //Scheduler& getScheduler();

private:
    ElevatorManager em;
    // std::vector<std::shared_ptr<FloorPanel>> floorPanels;
    // Scheduler scheduler;

};