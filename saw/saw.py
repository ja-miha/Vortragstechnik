import numpy as np
import random as rn


class SelfAvoidingWalk:
    def __init__(self):
        self.result = []
        self.possible_steps = [(1, 0, 0), (-1, 0, 0), (0, 1, 0), (0, -1, 0), (0, 0, 1), (0, 0, -1)]

    def make_all_steps(self, position, history):
        possiple_positions = {(position[0]+step[0], position[1]+step[1], position[2]+step[2]) for step in self.possible_steps}
        return possiple_positions-history

    def random_step(self, position):
        step = rn.choice(self.possible_steps)
        return (position[0]+step[0], position[1]+step[1], position[2]+step[2])

    def walk_everywhere(self, N, position=(0, 0, 0), history={(0, 0, 0)}, step=1):
        if step==N:
            for element in self.make_all_steps(position, history):
                self.result.append(element)
            return
        elif step==1:
            self.result = []
            nex = self.make_all_steps(position, history)
            for pos in nex:
                his=history.copy()
                his.add(pos)
                self.walk_everywhere(N, position=pos, history=his, step=step+1)
        else:
            nex = self.make_all_steps(position, history)
            for pos in nex:
                his=history.copy()
                his.add(pos)
                self.walk_everywhere(N, position=pos, history=his, step=step+1)

    def walk_sample(self, N, sample_size):
        self.result = []
        while len(self.result)<sample_size:
            position=(0, 0, 0)
            history={(0, 0, 0)}
            avoidance_flag = False
            for i in range(N):
                position = self.random_step(position)
                if position in history:
                        avoidance_flag = True
                        break
                history.add(position)
            if not avoidance_flag:
                self.result.append(position)

    def square_disp(self):
        s = 0
        for element in self.result:
            s += (element[0]**2 + element[1]**2 + element[2]**2)
        return s/len(self.result)




SAW = SelfAvoidingWalk()
SAW.walk_sample(5, 1000)
print(SAW.square_disp())
