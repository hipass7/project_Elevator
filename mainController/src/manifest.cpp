#include "manifest.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

ControllerConfig ControllerConfig::loadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open config file: " + path);
    }

    json j;
    file >> j;

    return ControllerConfig{
        j["num_floors"].get<int>(),
        j["num_elevators"].get<int>(),
        j["can_interface"].get<std::string>(),
        j["floor_panel_scan_interval_ms"].get<int>(),
        j["building_name"].get<std::string>()};
}