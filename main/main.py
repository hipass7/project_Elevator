from collections import deque
import time

ev_list = []
waiting_queue = deque([])
new_req = False
class Elevator:
    def __init__(self, number):
        self.now = 1 # int
        self.dest = Request(1, 0) # class Floor
        self.direction = 0 # down : -1, stop : 0, up : 1
        self.number = number
        self.internal_req = deque()
        self.open_door = False
        self.open_time = 0
        
    def internal_call(self, req):
        self.internal_req.append(req)
        
    def stop(self):
        if self.open_door:
            if time.time() - self.open_time > 5:
                self.open_door = False
                print(f"Elevator {self.number}: Door closed")
        else:
            self.open_door = True
            self.open_time = time.time()
            print(f"Elevator {self.number}: Door opened")
        
    def run(self):
        if not self.open_door:
            if self.dest.floor == self.now:
                if self.direction != 0:
                    print("ARRIVED")
                    self.stop()
                    self.direction = 0 # to do : internal_req feature -> self.dest.direction
                    return
                
            if self.direction != 0:
                select = False
                for i in waiting_queue:
                    if (self.now + self.direction == i.floor) and (self.direction == i.direction):
                        waiting_queue.appendleft(self.dest)
                        select = True
                        break
                if select:
                    self.dest = i
                    waiting_queue.remove(i)
                        
            else:
                if waiting_queue:
                    self.dest = waiting_queue.popleft()
                    if self.dest.floor > self.now:
                        self.direction = 1
                    else:
                        self.direction = -1
                else:
                    self.direction = 0
                    
            self.now += self.direction
        else:
            self.stop()
        
    def update(self):
        select = False
        if self.direction == 1:
            for i in waiting_queue:
                if self.dest.floor < i.floor:
                    select = True
                    waiting_queue.appendleft(self.dest)
                    self.dest = i
                    break            
        elif self.direction == -1:
            for i in waiting_queue:
                if self.dest.floor > i.floor:
                    select = True
                    waiting_queue.appendleft(self.dest)
                    self.dest = i
                    break              
        else:
            pass
        
        if select:
                waiting_queue.remove(i)

class Request:
    def __init__(self, floor, direction):
        self.floor = floor
        self.direction = direction
        self.req_time = time.time()
        
    def call_ev(self):
        global new_req
        waiting_queue.append(self)
        new_req = True
        
    def elapsed_time(self):
        return time.time() - self.req_time

if __name__ == "__main__":
    ev1, ev2 = Elevator(1), Elevator(2)
    ev_list.append(ev1)
    ev_list.append(ev2)
    
    req1 = Request(20, -1)
    req1.call_ev()
    
    req2 = Request(4, -1)
    req2.call_ev()
    
    req3 = Request(12, -1)
    req3.call_ev()
    
    req4 = Request(8, 1)
    req4.call_ev()
    
    while True:
        for i in ev_list:
            print(i.number, ">> dest :", i.dest.floor, "now :", i.now, "direction :", i.direction)
            i.run()
            if new_req:
                i.update()
            
        if new_req:
            new_req = False
        time.sleep(1)