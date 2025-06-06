cmake_minimum_required(VERSION 3.20)
project(BuildingControlSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif (MSVC)
    add_compile_options(/W4)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
