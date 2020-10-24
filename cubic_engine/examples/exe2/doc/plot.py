import csv
import matplotlib.pyplot as plt
from PIL import Image
from pylab import *


def main():

  dt = 0.5
  time = 0.0
  counter = 0

  with open('state.csv', 'r') as file:
    csv_reader = csv.reader(file, delimiter=",")

    x1 = []
    y1 = []

    x2 = []
    y2 = []

    for line in csv_reader:
          plt.cla()
          plt.gcf().canvas.mpl_connect('key_release_event',
							lambda event: [exit(0) if event.key == 'escape'
                                                   else None])

          if counter != 0:
              xe = float(line[0])
              ye = float(line[1])
              xa = float(line[2])
              ya = float(line[3])
              x1.append(xe)
              y1.append(ye)
              x2.append(xa)
              y2.append(ya)
              plt.plot(x1, y1, ".-g")
              plt.plot(x2, y2, ".-r")
              plt.legend(["EKF", "Exact Model"])
              plt.axis("equal")
              plt.title( "time %s " %time)
              plt.grid(True)
              filename = "plot_"+str(counter)+".png"
              plt.savefig(filename)
              plt.pause(0.001)
              time += dt
          counter += 1

if __name__ == '__main__':
	main()

	import imageio
	images = []
	filenames = [ "plot_"+str(counter+1)+".png" for counter in range(300) if counter % 10 == 0]

	for filename in filenames:
		images.append(imageio.imread(filename))
	imageio.mimsave('movie.gif', images)
