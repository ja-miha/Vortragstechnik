import numpy as np

#sample_size = 10000
#n_iter = 100000
#p_repete = 0.1

def random_walk(p_repete, p_start, sample_size=10000, n_iter=100000):
    rng = np.random.default_rng()
    x = np.zeros(sample_size, dtype=int)
    x = x + 2 * (rng.random(sample_size) < p_start) - 1
    history = (x == 1).astype("int")
    square_disp = [0, x.dot(x)]
    dist = [0, np.mean(x)]

    for i in range(1, n_iter):
            steps = 2 * (history > i * rng.random(sample_size)) - 1
            steps *= 2 * (rng.random(sample_size) < p_repete) - 1
            x = x + steps
            history = history + (steps == 1)#.astype("int")
            square_disp.append(x.dot(x))
            dist.append(np.mean(x))

    square_disp = np.array(square_disp) #/ sample_size
    square_disp = square_disp / sample_size
    dist = np.array(dist)
    np.savetxt("simres"+str(p_repete)+"_"+str(p_start)+".txt", square_disp)
    return square_disp, dist
