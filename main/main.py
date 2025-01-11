from collections import deque
import time

ev_list = []
waiting_queue = deque([])
new_req = False
class Elevator:
    def __init__(self, number):
        self.now = 1 # int
        self.dest = None # class Floor
        self.direction = 0 # down : -1, stop : 0, up : 1
        self.number = number
        self.internal_req = internalRequest()
        self.open_door = False
        self.open_time = 0
        
    def stop(self):
        if self.open_door:
            if ((time.time() - self.open_time > 5) or (self.internal_req.req_close)) and (not self.internal_req.req_open):
                self.open_door = False
                print(f"Elevator {self.number}: Door closed")
        else:
            if self.internal_req.req_queue:
                self.internal_req.req_queue.remove(self.dest)
            else:
                waiting_queue.remove(self.dest)
            self.open_door = True
            self.open_time = time.time()
            print(f"Elevator {self.number}: Door opened")

    def check(self):
        select = False
        for i in waiting_queue:
            if (self.now == i.floor) and (self.direction == i.direction):
                select = True
                break
        if select:
            waiting_queue.remove(i)
            self.stop()
            return True
        return False
    
    def internal_check(self):
        self.internal_req.req_queue.sort()
        for i in self.internal_req.req_queue:
            if (self.now >= i and self.direction == 1) or (self.now <= i and self.direction == -1):
                self.internal_req.req_queue.remove(i)
                print("INVALID REQUEST")

        if self.direction == 1:
            self.dest = self.internal_req.req_queue[0]
        elif self.direction == -1:
            self.dest = self.internal_req.req_queue[-1]
        
    def run(self):
        if self.dest is None:
            return
        
        # if self.internal_req.req_queue:
        #     self.internal_check()
        
        is_pass = False
        
        if not self.open_door:
            if self.dest.floor == self.now:
                if self.direction != 0:
                    print("ARRIVED")
                    self.stop()
                    self.direction = 0 # to do : internal_req feature -> self.dest.direction
                    return
                
            if self.direction != 0:
                is_pass = self.check()
                        
            else:
                if waiting_queue:
                    self.dest = waiting_queue.popleft()
                    if self.dest.floor > self.now:
                        self.direction = 1
                    else:
                        self.direction = -1
                else:
                    self.direction = 0

            if not is_pass:        
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
    def __init__(self, floor, direction, ev_assign = None):
        self.floor = floor
        self.direction = direction
        self.req_time = time.time()
        self.ev_assign = ev_assign
        
    def call_ev(self):
        global new_req
        waiting_queue.append(self)
        new_req = True
        
    def elapsed_time(self):
        return time.time() - self.req_time
    
class internalRequest:
    def __init__(self):
        self.req_queue = deque([])
        self.req_open = False
        self.req_close = False
        self.emergency = False
        
    def internal_call(self, req):
        self.req_queue.append(req)
        
    def open_door_1(self):
        self.req_open = True
        
    def open_door_0(self):
        self.req_open = False
        
    def close_door_1(self):
        self.req_close = True
        
    def close_door_0(self):
        self.req_close = False

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
            if i.dest is not None:
                print(i.number, ">> dest :", i.dest.floor, "now :", i.now, "direction :", i.direction)
            i.run()
            if new_req:
                i.update()
            
        if new_req:
            new_req = False
        time.sleep(1)