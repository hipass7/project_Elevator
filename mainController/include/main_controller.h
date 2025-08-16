#pragma once

#include <algorithm>
#include <climits>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "main_controller_can_interface.h"
#include "manifest.h"

class MainController
{
   public:
    explicit MainController(const std::string& config_path);
    void initialize();
    void printConfigSummary() const;
    void run();
    void scheduleElevators();

   private:
    ControllerConfig config;
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