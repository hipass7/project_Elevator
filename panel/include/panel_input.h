#pragma once
#include "panel_config.h"

class PanelInput {
public:
    explicit PanelInput(const PanelConfig& config);
    bool isUpPressed();
    bool isDownPressed();
#if defined(KEYBOARD_SIMULATION)
    char getNonBlockingChar();
#endif

private:
    int gpio_up;
    int gpio_down;
};

