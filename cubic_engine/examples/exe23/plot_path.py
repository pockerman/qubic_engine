"""
Utility script for plotting with matplotlib
"""
import matplotlib.pyplot as plt
import csv

def main(filename):
	
	with open(filename, 'r', newline='') as csvfile:
		csv_file_reader = csv.reader(csvfile, delimiter=",")
		
		row_count = 0
				
		v_x = []
		v_y = []
		
		path_x = [0.0, 10.0, 20.0, 50.0, 60., 90.0]
		path_y = [0.0,  0.0, 0.0,  30.0, 30.0, 0.0]
		
		for row in csv_file_reader:
		
			if row_count != 0:
				v_x.append(float(row[0]))
				v_y.append(float(row[1]))
					
			row_count += 1
		
		legend = []
		if len(cluster_0_x) != 0:
			plt.plot(cluster_0_x, cluster_0_y, 'b-')
			legend.append("Vehicle position")
		
		if len(cluster_1_x) != 0:
			plt.plot(path_x, path_y, 'ro-')
			legend.append("Path")
			
	  #legend.append("$\delta=3.5, k=0.1$")
			
		plt.xlabel('x')
		plt.ylabel('y')
		plt.legend(legend)

		plt.axis('square')
		plt.tight_layout(pad=0)
		plt.plot()
		plt.show()
	
if __name__ == '__main__':
	main("agent_state.csv")
	


