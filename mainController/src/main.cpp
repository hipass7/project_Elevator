// Building Control System

#include "buildingSystem.h"
//#include "building/Scheduler.h"
//#include "elevator/Elevator.h"
//#include "elevator/SafetySystem.h"
#include <memory>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    std::string config_path = "config/config.json";
    if (argc > 1) {
        config_path = argv[1];
    }
    // 건물 시스템 초기화
    BuildingSystem building(config_path);

    // 엘리베이터 생성 및 추가 (하드코딩 X -> 파일에서 불러오는 것으로 구현)
    // for (int i = 0; i < 3; ++i) {
    //     auto elevator = std::make_shared<Elevator>(i + 1); // 엘리베이터 ID 1부터 시작
    //     building.addElevator(elevator);
    // }

    building.initialize();

    building.run();

    return 0;
}
