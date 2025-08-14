#pragma once

#include "manifest.h"
#include "main_controller_can_interface.h"
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <algorithm>
#include <climits>

class MainController {
public:
    explicit MainController(const ControllerConfig& config);
    void initialize();
    void printConfigSummary() const;
    void run();
    void scheduleElevators();

private:
    std::string buildingName;
    int numFloors;
    int numElevators;
    int canRxId;
    int canTxIdBase;
    int scanIntervalMs;

    MainControllerCANInterface canInterface;
    std::map<int, ElevatorState> evMap{};
    std::vector<int> panelList{};
    std::pair<int, bool> panelRequest;
};