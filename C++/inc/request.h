#ifndef REQUEST_H
#define REQUEST_H

#include <deque>
#include <chrono>

class Request {
public:
    Request(int floor, int direction);

    int getFloor() const;
    int getDirection() const;
    void callElevator(std::deque<Request>& waitingQueue);
    double elapsedTime() const;

private:
    int floor;
    int direction;
    double reqTime; // Time when the request was created
};

#endif
