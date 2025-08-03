#include "panel_main.h"
#include <chrono>
#include <thread>
#include <csignal>
#include <iostream>

#if defined(KEYBOARD_SIMULATION)
#include "panel_input.h" // for getNonBlockingChar
#endif

static bool isRunning{true};

static void sigHandler(int32_t signum) {
    if (signum == SIGTERM || signum == SIGINT) {
        isRunning = false;
    }
}

PanelMain::PanelMain(const std::string& config_path)
    : config(PanelConfig::loadFromFile(config_path)),
      can(config), input(config), output(config) {}

void PanelMain::run() {
    signal(SIGINT, sigHandler);
    signal(SIGTERM, sigHandler);

    while (isRunning) {
#if defined(KEYBOARD_SIMULATION)
        char c = input.getNonBlockingChar();
        if (c == 'w' || c == 'W') {
            std::cout << "[SIM] UP key pressed (W)\n";
            can.sendButtonPress(true);
        } else if (c == 's' || c == 'S') {
            std::cout << "[SIM] DOWN key pressed (S)\n";
            can.sendButtonPress(false);
        }
#else
        if (input.isUpPressed()) {
            can.sendButtonPress(true);
        }
        if (input.isDownPressed()) {
            can.sendButtonPress(false);
        }
#endif

        can.receiveElevatorStatus();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

