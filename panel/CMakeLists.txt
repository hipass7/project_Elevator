cmake_minimum_required(VERSION 3.10)
project(panel_main)

set(CMAKE_CXX_STANDARD 17)

include_directories(include ${CMAKE_SOURCE_DIR}/../external)
file(GLOB SOURCES "src/*.cpp")

add_executable(panel_main ${SOURCES})

add_definitions(-DKEYBOARD_SIMULATION)

# SocketCAN 라이브러리 연결
#target_link_libraries(panel_main socketcan)

