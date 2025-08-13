#pragma once

#include "manifest.h"
#include "main_controller_can_interface.h"
#include <vector>
#include <memory>
#include <string>
#include <map>

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

    MainControllerCANInterface canInterface;
    std::map<int, std::vector<int>> evMap{};
    std::vector<int> panelList{};
    std::vector<std::pair<int, bool>> requests;
};