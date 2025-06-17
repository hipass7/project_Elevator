#pragma once

#include <vector>
#include <memory>

class FloorPanel {
public:
    // 기본 생성자
    FloorPanel();

    // 소멸자
    ~FloorPanel();

private:
    int floor;
    bool up;
    bool down;
};