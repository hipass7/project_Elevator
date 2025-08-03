#pragma once
#include <string>

struct PanelConfig {
    std::string can_interface;
    int floor;
    int elevator_id;
    int gpio_up_button;
    int gpio_down_button;
    int gpio_floor_led;
    int gpio_dir_led_up;
    int gpio_dir_led_down;

    static PanelConfig loadFromFile(const std::string& path);
};

