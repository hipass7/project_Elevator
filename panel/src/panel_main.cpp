#include "panel_main.h"
#include <chrono>
#include <thread>

PanelMain::PanelMain(const std::string& config_path)
    : config(PanelConfig::loadFromFile(config_path)),
      can(config), input(config), output(config) {}

void PanelMain::run() {
    while (true) {
        if (input.isUpPressed()) {
            can.sendButtonPress(true);
        }
        if (input.isDownPressed()) {
            can.sendButtonPress(false);
        }

        // 메인 컨트롤러에서 CAN 수신 → LED 표시 등
        can.receiveElevatorStatus();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

