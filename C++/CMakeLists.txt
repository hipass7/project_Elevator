cmake_minimum_required(VERSION 3.10)

project(elevatorSystem)

set(CMAKE_CXX_STANDARD 17)

# Add source files
set(SOURCES
    src/Elevator.cpp
    src/Request.cpp
    src/main.cpp
)

# Include header files
include_directories(inc)

# Create executable
add_executable(elevatorSystem ${SOURCES})
