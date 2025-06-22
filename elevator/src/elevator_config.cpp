#include "elevator_config.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

ElevatorConfig ElevatorConfig::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    nlohmann::json j;
    file >> j;

    return ElevatorConfig{
        j["id"].get<int>(),
        j["door_open_duration_sec"].get<int>(),
        j["disabled_buttons"].get<std::vector<int>>(),
        j["can_tx_id"].get<int>(),
        j["can_rx_id"].get<int>()
    };
}
