import csv
import matplotlib.pyplot as plt
from PIL import Image
from pylab import *



def main():
	
	dt = 0.5
	time = 0.0
	start_x = 40
	start_y = 740
	
	n_pixels_x = 771
	n_pixels_y = 772
	
	length_x = 1.0
	length_y = 1.0
	
	pixel_to_dist_x = length_x/n_pixels_x
	pixel_to_dist_y = length_y/n_pixels_y
	
	x = []
	y = []
	
	x_start = []
	y_start = []
	x_goal  = []
	y_goal  = []
	
	# read the image
	img = array(Image.open("map.png").convert('L'))
	
	# open the path file
	with open('path_000723.csv', 'r') as file:
		csv_reader = csv.reader(file, delimiter=",")
		
		
		for line in csv_reader:
			
			x_pos_pixels = float(line[0])/pixel_to_dist_x
			y_pos_pixels = float(line[1])/pixel_to_dist_y
			
			x1 = int(x_pos_pixels) #+ start_x
			y1 = n_pixels_y - int(y_pos_pixels) #+ start_y
			
			print(float(line[0]), ",",float(line[1]))
			print(x1, ",", y1)
			
			x.append(x1)
			y.append(y1)
			
	x_start.append(x[0])
	y_start.append(y[0])
	
	x_goal.append(x[len(x)-1])
	y_goal.append(y[len(y)-1])
			
	imshow(img)
                            	
	# plot the points with red star-markers
	plt.plot(x_start, y_start, 'ks:')
	plt.plot(x, y, '-r*')
	plt.plot(x_goal, y_goal, 'go')
	plt.legend(["Start", "Path", 'Goal' ])
	plt.title('Occupancy Grid')
	plt.show()
		
			
if __name__ == '__main__':
	main()	
