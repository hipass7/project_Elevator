#pragma once
#include <string>
#include <vector>

struct ElevatorConfig {
    int id;
    int doorOpenDurationSec;
    std::vector<int> disabledButtons;
};

struct BuildingConfig {
    int numFloors;
    int numElevators;
    std::vector<ElevatorConfig> elevators;
};

class ManifestLoader {
public:
    // 기본 생성자
    ManifestLoader();

    // 소멸자
    ~ManifestLoader();

    void loadFromFile(const std::string& filePath);

    BuildingConfig buildingConf;
    ElevatorConfig elevatorConf;
};
