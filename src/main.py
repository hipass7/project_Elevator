import multiprocessing
import time
import signal
import sys
import setproctitle


# 종료 플래그
running = True
g_up = False
g_down = False

# SIGINT 및 SIGTERM 핸들러
def signal_handler(signum, frame):
    global running
    print(f"Signal {signum} received, stopping...")
    running = False  # 종료 플래그를 False로 설정하여 while 루프 종료

# 사용자 정의 신호 핸들러
def signal_handler1(signum, frame):
    global g_up
    g_up = True
    print(f"Received signal: {signum}")

def signal_handler2(signum, frame):
    global g_down
    g_down = True
    print(f"Received signal: {signum}")

# 신호 핸들러 등록
signal.signal(signal.SIGINT, signal_handler)  # Ctrl+C를 처리
signal.signal(signal.SIGTERM, signal_handler)  # SIGTERM을 처리
signal.signal(signal.SIGUSR1, signal_handler1)  # 사용자 정의 신호를 처리
signal.signal(signal.SIGUSR2, signal_handler2)  # 사용자 정의 신호를 처리

# child만을 위한 함수
def child_target(shared_value):
    global running, g_up, g_down
    child_floor = shared_value[0]
    setproctitle.setproctitle("floor_" + str(child_floor))
    
    def up_trigger():
        if g_up:
            return "ON"

    def down_trigger():
        if g_down:
            return "ON"

    while running:
        if up_trigger() == "ON":
            shared_value[0] = child_floor
            shared_value[1] = True
            g_up = False

        if down_trigger() == "ON":
            shared_value[0] = child_floor
            shared_value[2] = True
            g_down = False

        time.sleep(1)

if __name__ == "__main__":
    processes = []
    max_floor = int(sys.argv[1])
    manager = multiprocessing.Manager()
    shared_values = []

    for i in range(max_floor):
        floor = (i + 1)
        shared_value = manager.list([floor, False, False])
        shared_values.append(shared_value)
        process = multiprocessing.Process(target=child_target, args=(shared_value,))
        processes.append(process)
        process.start()

    up_list = []
    down_list = []
    current_floor = 1
    direction = 0  # 1 for up, -1 for down, 0 for stopped

    # 알고리즘 최적화 필요
    while running:
        for shared_value in shared_values:
            if shared_value[1]:
                floor = shared_value[0]
                if floor not in up_list:
                    up_list.append(floor)
                shared_value[1] = False

            if shared_value[2]:
                floor = shared_value[0]
                if floor not in down_list:
                    down_list.append(floor)
                shared_value[2] = False

        if direction == 0:
            if up_list:
                direction = 1
            elif down_list:
                direction = -1

        if direction == 1:
            next_floor = min([floor for floor in up_list if floor >= current_floor], default=None)
            if next_floor is not None:
                up_list.remove(next_floor)
                while current_floor < next_floor:
                    current_floor += 1
                    print(f"Elevator at floor {current_floor} (going up)")
                    time.sleep(1)
                print(f"Elevator stopped at floor {current_floor}")
            else:
                direction = -1 if down_list else 0

        elif direction == -1:
            next_floor = max([floor for floor in down_list if floor <= current_floor], default=None)
            if next_floor is not None:
                down_list.remove(next_floor)
                while current_floor > next_floor:
                    current_floor -= 1
                    print(f"Elevator at floor {current_floor} (going down)")
                    time.sleep(1)
                print(f"Elevator stopped at floor {current_floor}")
            else:
                direction = 1 if up_list else 0

        # 현재 방향이 없는 경우, 가장 가까운 요청 처리

        if up_list or down_list:
            if up_list and (not down_list or min(up_list) < current_floor):
                next_floor = min(up_list, default=None)
                direction = 1
            elif down_list and (not up_list or max(down_list) > current_floor):
                next_floor = max(down_list, default=None)
                direction = -1
            if next_floor is not None:
                if direction == 1:
                    up_list.remove(next_floor)
                    while current_floor < next_floor:
                        current_floor += 1
                        print(f"Elevator at floor {current_floor} (going up)")
                        time.sleep(1)
                    print(f"Elevator stopped at floor {current_floor}")
                elif direction == -1:
                    down_list.remove(next_floor)
                    while current_floor > next_floor:
                        current_floor -= 1
                        print(f"Elevator at floor {current_floor} (going down)")
                        time.sleep(1)
                    print(f"Elevator stopped at floor {current_floor}")

        time.sleep(1)

    for process in processes:
        process.join()