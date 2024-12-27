from collections import deque
import time

ev_list = []
waiting_queue = deque([])
class Elevator:
    def __init__(self):
        self.now = 1 # int
        self.dest = Request(1, 1) # class Floor
        self.direction = 0 # down : -1, stop : 0, up : 1
        
    def run(self):
        if self.dest.floor == self.now:
            self.direction = 0
            print("ARRIVED")
            
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
                
        self.now += self.direction

class Request:
    def __init__(self, floor, direction):
        self.floor = floor
        self.direction = direction
        
    def call_ev(self):
        waiting_queue.append(self)

if __name__ == "__main__":
    ev1 = Elevator()
    
    req1 = Request(9, -1)
    req1.call_ev()
    
    req2 = Request(4, -1)
    req2.call_ev()
    
    while True:
        print("dest :", ev1.dest.floor, "now :", ev1.now, "direction :", ev1.direction)
        ev1.run()
        time.sleep(1)
    
    
    
# def new_request(req):
#     req_floor = req.floor
#     req_direction = req_direction

#     priority_min_list = []
#     for ev in ev_list:
#         if ev.direction == 1:
#             if ev.dest > req_floor > ev.now:
#                 priority_min_list.append(abs(ev.now - req_floor))
            
#     min_index = min_list.index(min(min_list))