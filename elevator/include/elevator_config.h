#pragma once
#include <vector>
#include <string>

struct ElevatorConfig {
    int id;
    int door_open_duration_sec;
    std::vector<int> disabled_buttons;
    int can_tx_id;
    int can_rx_id;
    std::string can_interface;

    static ElevatorConfig loadFromFile(const std::string& path);
};