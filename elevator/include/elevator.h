#pragma once
#include "elevator_can_interface.h"
#include "elevator_config.h"

class Elevator {
public:
    explicit Elevator(const std::string& config_path);
    void run();

private:
    ElevatorConfig config;
    ElevatorCANInterface can_interface;

    int current_floor;
    int dest_floor;
    int direction;
};