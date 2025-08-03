#pragma once
#include <vector>
#include <string>

struct ControllerConfig {
    int num_floors;
    int num_elevators;
    std::string can_interface;

    // 기타 빌딩 관련 설정들
    int floor_panel_scan_interval_ms; // floor panel 상태 체크 주기 등
    std::string building_name;
};

class ManifestLoader {
public:
    ManifestLoader();
    ManifestLoader(const std::string config);
    ~ManifestLoader();

    void loadFromFile(const std::string& filePath);
    const ControllerConfig& getConfig() const;

private:
    ControllerConfig config;
};
