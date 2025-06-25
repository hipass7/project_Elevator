#include "panel_input.h"
#include <iostream>

#if defined(KEYBOARD_SIMULATION)
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

PanelInput::PanelInput(const PanelConfig& config)
    : gpio_up(config.gpio_up_button), gpio_down(config.gpio_down_button) {}

bool PanelInput::isUpPressed() {
#if defined(KEYBOARD_SIMULATION)
    char c = getNonBlockingChar();
    if (c == 'w' || c == 'W') {
        std::cout << "[SIM] UP key pressed (W)\n";
        return true;
    }
    return false;
#else
    std::cout << "[GPIO] Reading UP button on GPIO " << gpio_up << "\n";
    return false; // 실제 GPIO 읽기 구현 필요
#endif
}

bool PanelInput::isDownPressed() {
#if defined(KEYBOARD_SIMULATION)
    char c = getNonBlockingChar();
    if (c == 's' || c == 'S') {
        std::cout << "[SIM] DOWN key pressed (S)\n";
        return true;
    }
    return false;
#else
    std::cout << "[GPIO] Reading DOWN button on GPIO " << gpio_down << "\n";
    return false;
#endif
}

#if defined(KEYBOARD_SIMULATION)
char PanelInput::getNonBlockingChar() {
    struct termios oldt, newt;
    char ch = 0;

    // 현재 터미널 설정 저장
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // 입력 버퍼 없이 즉시 키 입력 받기
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // 논블로킹 읽기 설정
    int oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    read(STDIN_FILENO, &ch, 1);

    // 설정 복구
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    return ch;
}
#endif
