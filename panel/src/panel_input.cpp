#include "panel_input.h"
#include <iostream>

#if defined(KEYBOARD_SIMULATION)
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

PanelInput::PanelInput(const PanelConfig& config)
    : gpio_up(config.gpio_up_button), gpio_down(config.gpio_down_button) {}

// 이 함수는 더 이상 외부에서 직접 사용되지 않습니다.
#if defined(KEYBOARD_SIMULATION)
char PanelInput::getNonBlockingChar() {
    struct termios oldt, newt;
    char ch = 0;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    read(STDIN_FILENO, &ch, 1);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    return ch;
}
#endif

bool PanelInput::isUpPressed() {
    // 실제 GPIO 로직으로 대체되어야 합니다.
    return false;
}

bool PanelInput::isDownPressed() {
    // 실제 GPIO 로직으로 대체되어야 합니다.
    return false;
}
