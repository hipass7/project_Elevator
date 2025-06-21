#pragma once

#include "manifest.h"
#include "elevator.h"
#include <vector>
#include <memory>
#include <string>

class MainController {
public:
    explicit MainController(const ControllerConfig& config);
    void initialize();
    void printConfigSummary() const;
    void run();

private:
    std::string buildingName;
    int numFloors;
    int numElevators;
    int canRxId;
    int canTxIdBase;
    int scanIntervalMs;

    std::vector<std::shared_ptr<Elevator>> elevators;
};