#pragma once
#include <vector>
#include <string>

struct ControllerConfig {
    int num_floors;
    int num_elevators;

    // 메인 컨트롤러 CAN ID (예: 버튼 이벤트 수신용 ID)
    int can_rx_id;      // floor panel -> main controller 메시지 수신용
    int can_tx_id_base; // main controller -> floor panel 상태 송신용 ID 베이스

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
