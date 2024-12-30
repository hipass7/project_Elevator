#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <deque>
#include "Request.h"

class Elevator {
public:
    Elevator(int number);

    void run(std::deque<Request>& waitingQueue);
    void stop();
    void move();
    bool checkArrival();
    void selectNextDestination(std::deque<Request>& waitingQueue);

private:
    int now; // Current floor
    Request dest; // Current destination
    int direction; // -1: down, 0: stop, 1: up
    int number; // Elevator ID
    bool openDoor;
    double openTime; // Time when the door opened
};

#endif
