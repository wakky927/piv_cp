from matplotlib import pyplot as plt
import numpy as np


def graph(path_dx, path_dy):
    fig = plt.figure(figsize=(16, 10))

    dx = np.loadtxt(path_dx, delimiter=',')
    dy = np.loadtxt(path_dy, delimiter=',')

    plt.quiver(dx, dy)
    plt.show()


if __name__ == '__main__':
    graph(path_dx="data/out/dx_0_1.csv", path_dy="data/out/dy_0_1.csv")
