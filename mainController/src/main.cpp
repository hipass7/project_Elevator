// Building Control System

#include "manifest.h"
#include "main_controller.h"

#include <memory>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    std::string config_path = "../../config/config.json";
    if (argc > 1) {
        config_path = argv[1];
    }
    ManifestLoader loader(config_path);
    ControllerConfig config = loader.getConfig();

    MainController controller(config);
    controller.printConfigSummary();
    controller.initialize();

    controller.run();

    return 0;
}
