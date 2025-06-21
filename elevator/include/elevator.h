#pragma once
#include "elevator_can_interface.h"
#include "manifest.h"

class Elevator {
public:
    explicit Elevator(const ElevatorConfig& config);
    void run();

private:
    ElevatorConfig config;
    ElevatorCANInterface can_interface;
    int current_floor;
};