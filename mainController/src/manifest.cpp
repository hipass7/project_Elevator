#include "manifest.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

ManifestLoader::ManifestLoader() = default;
ManifestLoader::ManifestLoader(const std::string config) {
    loadFromFile(config);
}

ManifestLoader::~ManifestLoader() = default;

void ManifestLoader::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filePath);
    }

    json j;
    file >> j;

    config.num_floors = j["num_floors"].get<int>();
    config.num_elevators = j["num_elevators"].get<int>();
    config.building_name = j["building_name"].get<std::string>();
    config.can_rx_id = j["can_rx_id"].get<int>();
    config.can_tx_id_base = j["can_tx_id_base"].get<int>();
    config.floor_panel_scan_interval_ms = j["floor_panel_scan_interval_ms"].get<int>();

    std::cout << "[ManifestLoader] Loaded config for building: " << config.building_name << "\n";
}

const ControllerConfig& ManifestLoader::getConfig() const {
    return config;
}