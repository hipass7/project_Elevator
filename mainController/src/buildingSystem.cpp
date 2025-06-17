#include "buildingSystem.h"
#include "manifest.h"
//#include "elevator.h"
//#include "scheduler.h"
//#include "floorPanel.h"

#include <iostream>

// 생성자
BuildingSystem::BuildingSystem() {
    std::cout << "[BuildingSystem] Constructor called." << std::endl;
    // 필요한 초기화가 있다면 여기에 작성
}

// 소멸자
BuildingSystem::~BuildingSystem() {
    std::cout << "[BuildingSystem] Destructor called." << std::endl;
}

// 시스템 초기화 (예: 엘리베이터 및 층 패널 초기화)
void BuildingSystem::initialize() {
    std::cout << "[BuildingSystem] Initialization started." << std::endl;

    ManifestLoader m;
    std::cout << m.buildingConf.numFloors << std::endl;

    // 파일을 불러옴 (ev들의 정보가 있는 json 파일 (list임))
    em.initialize();


    // 예시로 10층, 엘리베이터 2대 생성
    int numFloors = 10;
    int numElevators = 2;

    // for (int i = 0; i < numFloors; ++i) {
    //     floorPanels.push_back(std::make_shared<FloorPanel>(i));
    // }

    // for (int i = 0; i < numElevators; ++i) {
    //     elevators.push_back(std::make_shared<Elevator>(i));
    //     scheduler.registerElevator(elevators.back());
    // }

    std::cout << "[BuildingSystem] Initialization completed." << std::endl;
}

// 시스템 메인 루프 실행
void BuildingSystem::run() {
    std::cout << "[BuildingSystem] System running..." << std::endl;

    // 예시로 단순 루프를 사용
    // for (int tick = 0; tick < 10; ++tick) {
    //     std::cout << "Tick " << tick << std::endl;

    //     scheduler.step();  // 스케줄러에 명령 전달
    //     for (auto& elevator : elevators) {
    //         elevator->step();  // 각 엘리베이터 한 번 동작
    //     }
    // }

    std::cout << "[BuildingSystem] System stopped." << std::endl;
}
