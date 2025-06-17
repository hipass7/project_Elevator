#include "panel_main.h"

int main(int argc, char* argv[]) {
    std::string config_path = "config/panel_config.json";
    if (argc > 1) {
        config_path = argv[1];
    }

    PanelMain panel(config_path);
    panel.run();

    return 0;
}

