import csv
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
import seaborn as sns
from scipy.stats import kde
from sklearn.neighbors import KernelDensity
from scipy.stats import norm


def main(filename, legend):

  counter = 0

  with open(filename, 'r') as file:
    csv_reader = csv.reader(file, delimiter=",")

    x1 = []
    x2 = []

    for line in csv_reader:


          if counter > 1:
              t = float(line[0])
              p = float(line[1])

              x1.append(t)
              x2.append(p)

          counter += 1

    plt.plot(x1, x2, ".g")

    plt.xlabel("X1")
    plt.ylabel("X2")
    plt.title = legend
    plt.grid(True)
    plt.show()


if __name__ == '__main__':
  main("sample_1.csv", "m=0, sigma=1.0")
  main("sample_2.csv", "m=0, sigma=1.0")
