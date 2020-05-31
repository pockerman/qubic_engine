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

    xpos = []
    ypos = []


    for line in csv_reader:


          if counter > 1:
              time = float(line[0])
              x = float(line[1])
              y = float(line[2])
              xpos.append(x)
              ypos.append(y)


              filename = "plot_"+str(counter)+".png"
              #plt.savefig(filename)

              #time += dt
          counter += 1

    plt.plot(xpos, ypos, ".-g")
    plt.legend(["UKF"])
    plt.xlabel("X coordinate")
    plt.ylabel("Y coordinate")
    plt.axis("equal")
    #plt.title( "time %s " %time)
    plt.grid(True)
    plt.show()

if __name__ == '__main__':
	main()

	import imageio
	images = []
	#filenames = [ "plot_"+str(counter+1)+".png" for counter in range(300) if counter % 10 == 0]

	#for filename in filenames:
	#	images.append(imageio.imread(filename))
	#imageio.mimsave('movie.gif', images)
