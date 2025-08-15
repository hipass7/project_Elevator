#pragma once
#include <thread>

#include "elevator_can_interface.h"
#include "elevator_config.h"

enum class Direction : int8_t
{
    Stop = 1,
    Up = 2,
    Down = 0
};

class Elevator
{
   public:
    explicit Elevator(const std::string& config_path);
    ~Elevator();

    void initialize();  // 시스템 초기화
    void run();
    int checkCurrentFloor();
    void updateDirection();
    void stopAndOpenDoor();

    // 변수 getter
    int getCurrentFloor() const { return current_floor; }
    int getDestFloor() const { return dest_floor; }
    Direction getDirection() const { return direction; }
    bool isDoorOpen() const { return doorOpen; }

   private:
    ElevatorConfig config;
    ElevatorCANInterface can_interface;

    int current_floor;
    int dest_floor;
    Direction direction;
    bool doorOpen;

    std::thread canThread;  // 스레드 멤버
    bool running = true;    // 스레드 멤버

    void canListenerLoop();  // 스레드 멤버
};