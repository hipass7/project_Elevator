import multiprocessing
import time
import os
import sys
import signal

# 종료 플래그
running = True
up = False

# SIGINT 및 SIGTERM 핸들러
def signal_handler(signum, frame):
    global running
    print(f"Signal {signum} received, stopping...")
    running = False  # 종료 플래그를 False로 설정하여 while 루프 종료

# 사용자 정의 신호 핸들러
def signal_handler1(signum, frame):
    global up
    up = True
    print(f"Received signal: {signum}")

# 신호 핸들러 등록
signal.signal(signal.SIGINT, signal_handler)  # Ctrl+C를 처리
signal.signal(signal.SIGTERM, signal_handler)  # SIGTERM을 처리
signal.signal(signal.SIGUSR1, signal_handler1)  # 

# child만을 위한 함수를 만들어야함
def child_target(current_floor):
    global running, up
    while running:
        print(current_floor.value)
        if up_trigger() == "ON":
            current_floor.value += 1
            up = False
            pass

        if down_trigger() == "ON":
            pass

        time.sleep(1)



def up_trigger(): # for child
    if up: # signal to up
        return "ON"
        pass

def down_trigger(): # for child
    if 1: # signal to down
        # send pipe to parent
        pass



def spawn(i):
    pass
    # pid = os.fork()
    # if pid > 0:
    #     # parent
    #     return pid
        
    # elif pid == 0:
    #     # child
    #     while True:
    #         print(current_floor)
    #         time.sleep(1)
    #     exit(0)
        
    # else:
    #     print("Fork failed.")

if __name__ == "__main__":
    processes = []  # 프로세스를 저장할 리스트
    max_floor = sys.argv[1]
    shared_value = multiprocessing.Value('i', 1)
    for i in range(int(max_floor)):
        process = multiprocessing.Process(target=child_target, args=(shared_value,))
        processes.append(process)  # 프로세스 추가
        process.start()  # 프로세스 시작

    for process in processes:
        process.join()
    