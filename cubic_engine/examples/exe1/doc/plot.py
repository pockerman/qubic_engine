import csv
import matplotlib.pyplot as plt


def main():

  counter = 0

  with open('state.csv', 'r') as file:
    csv_reader = csv.reader(file, delimiter=",")

    time = []
    pos = []
    vel = []

    for line in csv_reader:


          if counter > 1:
              t = float(line[0])
              p = float(line[1])
              v = float(line[2])

              time.append(t)
              pos.append(p)
              vel.append(v)

          counter += 1

    plt.plot(time, pos, ".g")
    plt.plot(time, vel, ".r")
    plt.xlabel("Time")
    plt.legend(["Positon", "Velocity"])
    plt.grid(True)


if __name__ == '__main__':
	main()

