#include "panel_output.h"
#include <iostream>

PanelOutput::PanelOutput(const PanelConfig& config)
    : gpio_floor_led(config.gpio_floor_led),
      gpio_dir_up(config.gpio_dir_led_up),
      gpio_dir_down(config.gpio_dir_led_down) {}

void PanelOutput::setFloorLED(bool on) {
    std::cout << "[GPIO] Floor LED (" << gpio_floor_led << ") turned " << (on ? "ON" : "OFF") << "\n";
}

void PanelOutput::setDirectionLED(bool up, bool on) {
    int target_gpio = up ? gpio_dir_up : gpio_dir_down;
    std::string dir = up ? "UP" : "DOWN";
    std::cout << "[GPIO] Direction " << dir << " LED (" << target_gpio << ") turned " << (on ? "ON" : "OFF") << "\n";
}
