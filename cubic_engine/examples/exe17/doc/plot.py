"""
Utility script for plotting with matplotlib
"""
import matplotlib.pyplot as plt
import csv

def main(filename):
	
	with open(filename, 'r', newline='') as csvfile:
		csv_file_reader = csv.reader(csvfile, delimiter=",")
		
		row_count = 0
				
		cluster_0_x = []
		cluster_0_y = []
		
		cluster_1_x = []
		cluster_1_y = []
		
		cluster_2_x = []
		cluster_2_y = []
		
		for row in csv_file_reader:
		
			if row_count != 0:
				cid = int(row[0])
		
				if cid == 0:
					cluster_0_x.append(float(row[1]))
					cluster_0_y.append(float(row[2]))
				elif cid == 1:
					cluster_1_x.append(float(row[1]))
					cluster_1_y.append(float(row[2]))
				elif cid == 2:
					cluster_2_x.append(float(row[1]))
					cluster_2_y.append(float(row[2]))
				else:
					raise ValueError("Invlaid Cluster Id")
					
			row_count += 1
		
		legend = []
		if len(cluster_0_x) != 0:
			plt.plot(cluster_0_x, cluster_0_y, 'ko')
			legend.append("Cluster 0")
		
		if len(cluster_1_x) != 0:
			plt.plot(cluster_1_x, cluster_1_y, 'ro')
			legend.append("Cluster 1")
			
		if len(cluster_2_x) != 0:
			plt.plot(cluster_2_x, cluster_2_y, 'bo')
			legend.append("Cluster 2")
			
		plt.xlabel('x')
		plt.ylabel('y')
		plt.legend(legend)

		plt.axis('square')
		plt.tight_layout(pad=0)
		plt.plot()
		plt.show()
	
if __name__ == '__main__':
	main("kmeans_example.csv")
	main("kmeans_normal_example.csv")
