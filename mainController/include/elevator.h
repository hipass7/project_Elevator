#pragma once

#include "manifest.h"
#include <string>
#include <vector>
#include <iostream>

class Elevator {
public:
    explicit Elevator(const ElevatorConfig& config);

    void printStatus() const;

private:
    int id;
    int doorOpenDurationSec;
    std::vector<int> disabledButtons;
    int canTxId;
    int canRxId;
};
