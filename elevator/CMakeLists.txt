cmake_minimum_required(VERSION 3.14)
project(elevator_main)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(elevator_main
    src/main.cpp  # <-- main 함수가 포함된 파일
    src/elevator.cpp
    src/elevator_can_interface.cpp
    src/elevator_config.cpp
)

target_include_directories(elevator_main PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../external
)

# if windows, deactive
target_link_libraries(elevator_main
    PRIVATE
        pthread
)