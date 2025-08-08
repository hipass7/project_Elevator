# Dockerfile for building and running CAN socket C++ app with latest can-utils (from GitHub)

FROM rockylinux:8

# 1. 기본 빌드 도구, CAN 관련 헤더, git 등 설치
RUN dnf -y update && dnf -y install \
    gcc gcc-c++ make cmake git autoconf automake libtool pkgconfig net-tools iproute \
    kernel-headers kernel-devel \
    && dnf clean all

# 2. 최신 can-utils 소스 클론 (2025-08-08 기준 최신 경로)
RUN git clone https://github.com/linux-can/can-utils.git /usr/src/can-utils

# 3. can-utils 빌드 및 설치
WORKDIR /usr/src/can-utils
RUN make && make install

# 4. 작업 디렉터리 설정 (소스 복사 후 빌드 예상)
WORKDIR /app

# 5. 필요하다면 여기에 소스 복사 (예: main_controller_can_interface.cpp 등)
# COPY . /app

# 6. 기본 커맨드 (bash 실행)
CMD ["/bin/bash"]
