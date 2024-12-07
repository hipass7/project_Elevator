
ev_list = []
class Elevator:
    def __init__(self):
        self.now = 1
        self.queue = []
        self.dest = 0
        self.direction = 0

class Request:
    def __init__(self, floor, direction):
        self.floor = floor
        self.direction = direction

def new_request(req):
    req_floor = req.floor
    req_direction = req_direction

    priority_min_list = []
    for ev in ev_list:
        if ev.direction == 1:
            if ev.dest > req_floor > ev.now:
                priority_min_list.append(abs(ev.now - req_floor))
            

        

    min_index = min_list.index(min(min_list))



if __name__ == "__main__":
    a, b, c, d = Elevator(), Elevator(), Elevator(), Elevator()
    ev_list = [a, b, c, d]