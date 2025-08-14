#pragma once
#include "elevator_can_interface.h"
#include "elevator_config.h"
#include <thread>

enum class Direction : int8_t {
    Stop = 1,
    Up   = 2,
    Down = 0
};

class Elevator {
public:
    explicit Elevator(const std::string& config_path);
    ~Elevator();
    void run();
    int checkCurrentFloor();
    void updateDirection();
    void stopAndOpenDoor();

private:
    ElevatorConfig config;
    ElevatorCANInterface can_interface;

    int current_floor = 1;
    int dest_floor = -1;
    Direction direction = Direction::Stop;;
    bool doorOpen = false;
    std::thread canThread;  // 스레드 멤버
    bool running = true; // 스레드 멤버

    void canListenerLoop(); // 스레드 멤버
};