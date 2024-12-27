from collections import deque
import time

ev_list = []
waiting_queue = deque([])
class Elevator:
    def __init__(self, number):
        self.now = 1 # int
        self.dest = Request(1, 1) # class Floor
        self.direction = 0 # down : -1, stop : 0, up : 1
        self.number = number
        
    def run(self):
        if self.dest.floor == self.now:
            if self.direction != 0:
                print("ARRIVED")
                self.direction = 0
            
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
    ev1, ev2 = Elevator(1), Elevator(2)
    ev_list.append(ev1)
    ev_list.append(ev2)
    
    req1 = Request(9, -1)
    req1.call_ev()
    
    req2 = Request(4, -1)
    req2.call_ev()
    
    req3 = Request(15, 1)
    req3.call_ev()
    
    req4 = Request(8, 1)
    req4.call_ev()
    
    while True:
        for i in ev_list:
            print(i.number, ">> dest :", i.dest.floor, "now :", i.now, "direction :", i.direction)
            i.run()
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