#include "elevator.h"
#include "request.h"
#include <deque>
#include <thread>
#include <chrono>

int main() {
    std::deque<Request> waitingQueue;
    Elevator elevator(1);

    // Create and call requests
    Request r1(5, 1);
    r1.callElevator(waitingQueue);

    Request r2(2, -1);
    r2.callElevator(waitingQueue);

    // Run elevator
    for (int i = 0; i < 10; ++i) {
        elevator.run(waitingQueue);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
