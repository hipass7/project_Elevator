#pragma once
#include "panel_config.h"
#include "panel_can_interface.h"
#include "panel_input.h"
#include "panel_output.h"

class PanelMain {
public:
    explicit PanelMain(const std::string& config_path);
    void run();

private:
    PanelConfig config;
    PanelCANInterface can;
    PanelInput input;
    PanelOutput output;
};

