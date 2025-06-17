#include "floorPanel.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

// 생성자
FloorPanel::FloorPanel() {
    std::cout << "[Elevator] Constructor called." << std::endl;
    // 필요한 초기화가 있다면 여기에 작성
}

// 소멸자
FloorPanel::~FloorPanel() {
    std::cout << "[ElevatorManager] Destructor called." << std::endl;
}

struct BoardConfig {
    int floor;
    int can_id;
    bool has_up;
    bool has_down;
    bool use_override;
};

BoardConfig loadConfigFromFile(const std::string& path) {
    std::ifstream in(path);
    nlohmann::json j;
    in >> j;

    return {
        j["floor"],
        j["can_id"],
        j["direction_buttons"]["up"],
        j["direction_buttons"]["down"],
        j["use_override"]
    };
}
