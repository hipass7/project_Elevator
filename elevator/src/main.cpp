#include <iostream>
#include <stdexcept>

#include "elevator.h"

int main(int argc, char* argv[])
{
    std::string config_path = "../config/ev_config.json";
    if (argc > 1)
    {
        config_path = argv[1];
    }

    try
    {
        Elevator elevator(config_path);
        elevator.initialize();  // Initialize the elevator
        std::cout << "[Elevator] Starting elevator operations...\n";
        elevator.run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}