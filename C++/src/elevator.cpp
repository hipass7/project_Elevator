#include "elevator.h"
#include <iostream>
#include <chrono>
#include <thread>

Elevator::Elevator(int number)
    : now(1), dest(Request(1, 0)), direction(0), number(number), openDoor(false), openTime(0) {}

void Elevator::stop() {
    if (openDoor) {
        double currentTime = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        if (currentTime - openTime > 5) {
            openDoor = false;
            std::cout << "Elevator " << number << ": Door closed\n";
        }
    } else {
        openDoor = true;
        openTime = std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
        std::cout << "Elevator " << number << ": Door opened\n";
    }
}

void Elevator::move() {
    if (direction != 0) {
        now += direction;
        std::cout << "Elevator " << number << ": Moving to floor " << now << "\n";
    }
}

bool Elevator::checkArrival() {
    if (dest.getFloor() == now) {
        std::cout << "Elevator " << number << ": Arrived at floor " << now << "\n";
        stop();
        direction = 0;
        return true;
    }
    return false;
}

void Elevator::selectNextDestination(std::deque<Request>& waitingQueue) {
    if (!waitingQueue.empty()) {
        dest = waitingQueue.front();
        waitingQueue.pop_front();
        direction = dest.getFloor() > now ? 1 : -1;
        std::cout << "Elevator " << number << ": New destination " << dest.getFloor() << "\n";
    } else {
        direction = 0;
    }
}

void Elevator::run(std::deque<Request>& waitingQueue) {
    if (openDoor) {
        stop();
    } else {
        if (!checkArrival()) {
            move();
            selectNextDestination(waitingQueue);
        }
    }
}
