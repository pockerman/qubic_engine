import csv
import matplotlib.pyplot as plt
from PIL import Image
from pylab import *

def main():
  dt = 0.1
  time = 0.0

  n_pixels_x = 431
  n_pixels_y = 430

  length_x = 10.0
  length_y = 10.0

  pixel_to_dist_x = length_x/n_pixels_x
  pixel_to_dist_y = length_y/n_pixels_y

  img = array(Image.open('map_I.png').convert('L'))

  x_start = []
  y_start = []
  x_goal  = []
  y_goal  = []

  # load the path
  xpath=[]
  ypath=[]
  with open('path.csv', 'r') as file:
    csv_reader = csv.reader(file, delimiter=",")
    for line in csv_reader:
      x_pos_pixels = float(line[0])/pixel_to_dist_x
      y_pos_pixels = float(line[1])/pixel_to_dist_y

      x1 = int(x_pos_pixels) #+ start_x
      y1 = n_pixels_y - int(y_pos_pixels) #+ start_y

      xpath.append(float(x1))
      ypath.append(float(y1))

  # the start and goal coordinates
  x_start.append(xpath[0])
  y_start.append(ypath[0])

  x_goal.append(xpath[len(xpath)-1])
  y_goal.append(ypath[len(ypath)-1])

  xstate=[]
  ystate=[]


  with open('state.csv', 'r') as file:


    csv_reader = csv.reader(file, delimiter=",")


    counter = 0
    img_counter =0
    for line in csv_reader:
      if counter > 1:

        plt.cla()
        plt.gcf().canvas.mpl_connect('key_release_event',
                        lambda event: [exit(0) if event.key == 'escape' else None])

        x_pos_pixels = float(line[0])/pixel_to_dist_x
        y_pos_pixels = float(line[1])/pixel_to_dist_y



        x1 = int(x_pos_pixels) #+ start_x
        y1 = n_pixels_y - int(y_pos_pixels)

        xstate.append(float(x1))
        ystate.append(float(y1))


        imshow(img)
        plt.plot(x_start, y_start, 'ks:')
        plt.plot(xpath, ypath, 'b-')
        plt.plot(x_goal, y_goal, 'ro')
        plt.plot(xstate,ystate,'g*')
        plt.legend(["START", "Path", "GOAL", "State"])
        plt.title('Occupancy Grid, Time {0}'.format(round(time,3)))
        filename = "plot_"+str(img_counter)+".png"
        plt.savefig(filename)
        plt.pause(0.001)
        time += dt
        img_counter += 1

      counter += 1


if __name__ == '__main__':
  main()
  import imageio
  images = []
  filenames = [ "plot_"+str(counter+1)+".png" for counter in range(140) if counter % 10 == 0]

  for filename in filenames:
    images.append(imageio.imread(filename))
  imageio.mimsave('movie.gif', images)
