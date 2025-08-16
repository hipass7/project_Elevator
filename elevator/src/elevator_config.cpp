#include "elevator_config.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ElevatorConfig ElevatorConfig::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open config file: " + path);
    }
    json j;
    file >> j;

    return ElevatorConfig{
        j["id"].get<int>(),
        j["door_open_duration_sec"].get<int>(),
        j["disabled_buttons"].get<std::vector<int>>(),
        j["can_interface"].get<std::string>()};
}
