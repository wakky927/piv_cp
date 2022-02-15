import numpy as np
from matplotlib import pyplot as plt


def main():
    width, height = 570, 512
    n = 48

    x, y = np.meshgrid(np.linspace(0, width, n, dtype="int"),
                       np.linspace(0, height, n, dtype="int"))

    dx = np.loadtxt("data/out/dx_1_2.csv", delimiter=',')
    dy = np.loadtxt("data/out/dy_1_2.csv", delimiter=',')

    fig, ax = plt.subplots(figsize=(12, 10))
    ax.quiver(x, y, dx, -dy)
    ax.invert_yaxis()

    plt.show()


if __name__ == '__main__':
    main()
