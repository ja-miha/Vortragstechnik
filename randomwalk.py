import numpy as np

#sample_size = 10000
#n_iter = 100000
#p_repete = 0.1

def random_walk(sample_size, n_iter, p_repete):
    rng = np.random.default_rng()
    x = np.zeros(sample_size, dtype=int)
    x = x + 2 * rng.integers(2, size = sample_size) - 1
    history = (x == 1).astype("int")
    square_disp = [0, x.dot(x)]

    for i in range(1, n_iter):
            steps = 2 * (history > i * rng.random(sample_size)) - 1
            steps *= 2 * (rng.random(sample_size) < p_repete) - 1
            x = x + steps
            history = history + (steps == 1)#.astype("int")
            square_disp.append(x.dot(x))

    square_disp = np.array(square_disp) #/ sample_size
    np.savetxt("simres"+str(p_repete)+".txt", square_disp)
