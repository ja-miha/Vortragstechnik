import numpy as np

ls = []

def make_step(position, history):
    possible_steps = [(1, 0, 0), (-1, 0, 0), (0, 1, 0), (0, -1, 0), (0, 0, 1), (0, 0, -1)]
    possiple_positions = {(position[0]+step[0], position[1]+step[1], position[2]+step[2]) for step in possible_steps}
    return possiple_positions-history


def saw(N, position=(0, 0, 0), history={(0, 0, 0)}, step=1):
    global ls
    if step==N:
        for element in make_step(position, history):
            ls.append(element)
        return
    else:
        nex = make_step(position, history)
        for pos in nex:
            his=history.copy()
            his.add(pos)
            saw(N, position=pos, history=his, step=step+1)

saw(10)
#print(ls)
print(len(ls))