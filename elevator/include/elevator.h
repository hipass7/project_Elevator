#pragma once
#include "elevator_can_interface.h"
#include "elevator_config.h"

enum class Direction : int8_t {
    Stop = 0,
    Up   = 1,
    Down = -1
};

class Elevator {
public:
    explicit Elevator(const std::string& config_path);
    void run();
    int checkCurrentFloor();
    void updateDirection();
    void stopAndOpenDoor();

private:
    ElevatorConfig config;
    ElevatorCANInterface can_interface;

    int current_floor;
    int dest_floor;
    Direction direction;
    bool doorOpen;
};