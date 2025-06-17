#include "panel_input.h"
#include <iostream>

PanelInput::PanelInput(const PanelConfig& config)
    : gpio_up(config.gpio_up_button), gpio_down(config.gpio_down_button) {}

bool PanelInput::isUpPressed() {
    std::cout << "[GPIO] Reading UP button on GPIO " << gpio_up << "\n";
    // 실제로는 GPIO 핀 상태를 읽어야 함
    return false; // 예시: 버튼 안 눌림
}

bool PanelInput::isDownPressed() {
    std::cout << "[GPIO] Reading DOWN button on GPIO " << gpio_down << "\n";
    return false;
}
