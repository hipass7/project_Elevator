#include "request.h"
#include <iostream>

Request::Request(int floor, int direction)
    : floor(floor), direction(direction) {
    reqTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

int Request::getFloor() const {
    return floor;
}

int Request::getDirection() const {
    return direction;
}

void Request::callElevator(std::deque<Request>& waitingQueue) {
    waitingQueue.push_back(*this);
    std::cout << "Request: Floor " << floor << ", Direction " << direction << "\n";
}

double Request::elapsedTime() const {
    double currentTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
    return currentTime - reqTime;
}
