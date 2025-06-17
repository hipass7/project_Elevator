#pragma once

#include <vector>
#include <memory>
#include "elevator.h"
class Scheduler; // #include "scheduler.h"
class FloorPanel; // #include "floorPanel.h"

class ElevatorManager {
public:
    // 기본 생성자
    ElevatorManager();

    // 소멸자
    ~ElevatorManager();

    void initialize();

    void run();

    // 엘리베이터 추가
    //void addElevator(std::shared_ptr<Elevator> elevator);

    // 스케줄러 접근자
    //Scheduler& getScheduler();

private:
    std::vector<std::shared_ptr<Elevator>> elevators;

};