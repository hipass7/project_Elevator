#include "manifest.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// 생성자
ManifestLoader::ManifestLoader() {
    std::cout << "[Elevator] Constructor called." << std::endl;
    loadFromFile("config/config.json");
}

// 소멸자
ManifestLoader::~ManifestLoader() {
}

void ManifestLoader::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filePath);
    }

    json j;
    file >> j;

    buildingConf.numFloors = j["num_floors"];
    buildingConf.numElevators = j["num_elevators"];

    for (const auto& elev : j["elevators"]) {
        ElevatorConfig e;
        elevatorConf.id = elev["id"];
        elevatorConf.disabledButtons = elev["disabled_buttons"].get<std::vector<int>>();
        elevatorConf.doorOpenDurationSec = elev["door_open_duration_sec"];
        buildingConf.elevators.push_back(e);
    }
}