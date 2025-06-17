#pragma once
#include "panel_config.h"

class PanelOutput {
public:
    explicit PanelOutput(const PanelConfig& config);
    void setFloorLED(bool on);
    void setDirectionLED(bool up, bool on);

private:
    int gpio_floor_led;
    int gpio_dir_up;
    int gpio_dir_down;
};

