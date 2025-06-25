#include "panel_main.h"
#include <chrono>
#include <thread>
#include <csignal>

static bool isRunning{true};

static void sigHandler(int32_t signum) {
    if (signum == SIGTERM) {
        isRunning = false;
    }
}

PanelMain::PanelMain(const std::string& config_path)
    : config(PanelConfig::loadFromFile(config_path)),
      can(config), input(config), output(config) {}

void PanelMain::run() {
    while (isRunning) {
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

