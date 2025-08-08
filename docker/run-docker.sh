#!/bin/bash

set -e

# 기본값
CONTAINER_NAME="project-elevator"
IMAGE_NAME="localhost/project-elevator"
WORKDIR="/app"

# 옵션 파싱
while getopts "c:" opt; do
    case "$opt" in
        c) CONTAINER_NAME="$OPTARG" ;;
        *) echo "Usage: $0 [-c container_name]"; exit 1 ;;
    esac
done

# 컨테이너 실행
docker run -it --rm --privileged --network=host \
    --name "$CONTAINER_NAME" \
    -v "$(pwd)/..:$WORKDIR:Z" \
    "$IMAGE_NAME" \
    /bin/bash
