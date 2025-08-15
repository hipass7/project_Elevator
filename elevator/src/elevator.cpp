#include "elevator.h"

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

constexpr std::chrono::milliseconds kLoopInterval(100);
constexpr int kNoDestination = -1;
constexpr int kInitialFloor = 1;

Elevator::Elevator(const std::string& config_path)
    : config(ElevatorConfig::loadFromFile(config_path)),
      can_interface(*this, config),
      current_floor(kInitialFloor),
      dest_floor(kNoDestination),
      direction(Direction::Stop),
      doorOpen(false)
{
    std::cout << "[Elevator] Initialized elevator ID " << config.id << "\n";
    canThread = std::thread(&Elevator::canListenerLoop, this);
}

Elevator::~Elevator()
{
    running = false;
    if (canThread.joinable())
    {
        canThread.join();
    }
}

void Elevator::canListenerLoop()
{
    int dest = -1;
    while (running)
    {
        if (can_interface.receiveCommand(dest))
        {
            if (dest == 0)
            {
                dest_floor = kNoDestination;  // 0은 정지 명령
                direction = Direction::Stop;
                std::cout << "[CAN] Received stop command.\n";
            }
            else
            {
                dest_floor = dest;
            }
            updateDirection();
            std::cout << "[CAN] Received destination: " << dest_floor << std::endl;
        }
    }
}

void Elevator::initialize()
{
    std::cout << "[Elevator] Initializing elevator...\n";
    if (!can_interface.sendCommand(ElevatorState::Initialize))
    {  // 초기화 명령 전송
        std::cerr << "[Elevator] Failed to send initial status command.\n";
        return;
    }
    if (!can_interface.sendCommand(ElevatorState::Status))
    {
        std::cerr << "[Elevator] Failed to send initial status command.\n";
        return;
    }
    std::cout << "[Elevator] Initialization complete.\n";
}

void Elevator::run()
{
    std::cout << "[Elevator] Running elevator logic...\n";
    while (true)
    {
        // 1. 새로운 엘리베이터 위치 파악 (센서 input)
        int now_loc = checkCurrentFloor();

        // 2. 기존 위치와 새로 파악한 위치가 다를 경우, 상태 update
        if (now_loc != current_floor)
        {
            current_floor = now_loc;
            updateDirection();

            // 3. update면 send
            can_interface.sendCommand(ElevatorState::Status);
        }

        // 4. dest 도착인지 확인
        if (current_floor == dest_floor)
        {
            // 5. true면 door 열기
            stopAndOpenDoor();
        }

        std::this_thread::sleep_for(kLoopInterval);  // 짧은 대기 (CPU 사용량에 따라 조절 예정 / 임시)
    }
}

int Elevator::checkCurrentFloor()
{
    // 센서로 확인 (지금은 임시)
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // HAL_readSensor() 대체
    return current_floor + static_cast<int8_t>(direction) - 1;     // return HAL_readSensor();
}

void Elevator::updateDirection()
{
    if (dest_floor == kNoDestination) return;
    if (current_floor < dest_floor)
        direction = Direction::Up;
    else if (current_floor > dest_floor)
        direction = Direction::Down;
    else
        direction = Direction::Stop;
}

void Elevator::stopAndOpenDoor()
{
    direction = Direction::Stop;
    std::this_thread::sleep_for(std::chrono::seconds(1));  // 문 열리는데 걸리는 시간
    doorOpen = true;

    if (can_interface.sendCommand(ElevatorState::Status))
    {
        std::cout << "[Elevator] Door opened at floor " << current_floor << ".\n";
    }
    else
    {
        std::cerr << "[Elevator] Failed to send door open command.\n";
    }

    // '문 열림' 명령 처리
    std::cout << "[INFO] Arrived at floor " << current_floor << ". Door opening...\n";
    std::this_thread::sleep_for(std::chrono::seconds(config.door_open_duration_sec));
    std::cout << "[Elevator] Door closed.\n";
    doorOpen = false;

    if (can_interface.sendCommand(ElevatorState::Status))
    {
        std::cout << "[Elevator] Door closed at floor " << current_floor << ".\n";
    }
    else
    {
        std::cerr << "[Elevator] Failed to send door close command.\n";
    }

    // 실제 문 열기/정지 로직 삽입
}