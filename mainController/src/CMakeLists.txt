# src/CMakeLists.txt

# 모든 cpp 파일을 수집
file(GLOB_RECURSE SRC_FILES "*.cpp")

# 실행 파일 생성
add_executable(elevatorMain ${SRC_FILES})

# include 디렉토리 추가
target_include_directories(elevatorMain
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/external
)

# 필요한 라이브러리 링크 (예: pthread 등)
target_link_libraries(elevatorMain
    PRIVATE
        # 예시: pthread
        # pthread
)
