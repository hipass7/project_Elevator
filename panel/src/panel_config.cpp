#include "panel_config.h"
#include <nlohmann/json.hpp>
#include <fstream>

PanelConfig PanelConfig::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    nlohmann::json j;
    file >> j;

    return PanelConfig{
        j["can_interface"],
        j["floor"],
        j["can_tx_id"],
        j["can_rx_id"],
        j["gpio_up_button"],
        j["gpio_down_button"],
        j["gpio_floor_led"],
        j["gpio_dir_led_up"],
        j["gpio_dir_led_down"]
    };
}
