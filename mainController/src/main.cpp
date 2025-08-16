// Building Control System

#include <iostream>
#include <memory>
#include <vector>

#include "main_controller.h"
#include "manifest.h"

int main(int argc, char* argv[])
{
    std::string config_path = "../../config/config.json";
    if (argc > 1)
    {
        config_path = argv[1];
    }
    MainController controller(config_path);
    controller.printConfigSummary();
    controller.initialize();

    controller.run();

    return 0;
}
