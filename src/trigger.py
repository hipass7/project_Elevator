import os
import signal
import sys


if __name__ == "__main__":
    pid = sys.argv[1]
    direction = sys.argv[2]

    # 자식 프로세스에 사용자 정의 신호 보내기
    if int(direction) == 1:
        os.kill(int(pid), signal.SIGUSR1)
    elif int(direction) == -1:
        os.kill(int(pid), signal.SIGUSR2)
    else:
        print("Invalid argument")
