//#include "elevator.h"
//#include "scheduler.h"
//#include "floorPanel.h"
#include "elevatorManager.h"

#include <iostream>

// 생성자
ElevatorManager::ElevatorManager() {
    std::cout << "[ElevatorManager] Constructor called." << std::endl;
    // 필요한 초기화가 있다면 여기에 작성
}

// 소멸자
ElevatorManager::~ElevatorManager() {
    std::cout << "[ElevatorManager] Destructor called." << std::endl;
}

// 시스템 초기화 (예: 엘리베이터 및 층 패널 초기화)
void ElevatorManager::initialize() {
    std::cout << "[ElevatorManager] Initialization started." << std::endl;

    // 파일을 불러옴 (ev들의 정보가 있는 json 파일 (list임))
    // 폴더 내에서 모든 json loading (ev_id.json)
    // auto evManifest = load(folder) // evManifest = ["68.json", ...]
    // 예시로 10층, 엘리베이터 2대 생성
    int numFloors = 10;
    int numElevators = 2; // folder.size()

    // for (int i = 0; i < numElevators; ++i) {
    //     // auto manifest = open(evManifest[i])
    //     // manifest[id] 
    //     auto elevator = std::make_shared<Elevator>();
    //     elevators.push_back(std::make_shared<Elevator>(i));
    //     scheduler.registerElevator(elevators.back());
    // }

    std::cout << "[ElevatorManager] Initialization completed." << std::endl;
}

// 시스템 메인 루프 실행
void ElevatorManager::run() {
    std::cout << "[ElevatorManager] System running..." << std::endl;

    // 예시로 단순 루프를 사용
    // for (int tick = 0; tick < 10; ++tick) {
    //     std::cout << "Tick " << tick << std::endl;

    //     scheduler.step();  // 스케줄러에 명령 전달
    //     for (auto& elevator : elevators) {
    //         elevator->step();  // 각 엘리베이터 한 번 동작
    //     }
    // }

    std::cout << "[ElevatorManager] System stopped." << std::endl;
}
