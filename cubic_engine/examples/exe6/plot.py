"""
Utility script for plotting with matplotlib
"""
import matplotlib.pyplot as plt
import csv
import math

def main(filename):
	
  with open(filename, 'r', newline='') as csvfile:
    csv_file_reader = csv.reader(csvfile, delimiter=",")
		
    row_count = 0
    cluster_0_x = []
    cluster_0_y = []
		
    for row in csv_file_reader:
		
      if row_count > 1:
        cluster_0_x.append(float(row[0]))
        cluster_0_y.append(float(row[1]))
      row_count += 1
		
    legend = []
    if len(cluster_0_x) != 0:
      plt.plot(cluster_0_x, cluster_0_y, 'r-')
      legend.append("Episode/Reward")
			
	  #legend.append("$\delta=3.5, k=0.1$")
			
    plt.xlabel('Episode')
    plt.ylabel('Sum of rewards')
    plt.legend(legend)

    plt.axis('square')
    #plt.xlim(0, 10000)
    plt.tight_layout(pad=0)
    plt.plot()
    plt.show()
	
if __name__ == '__main__':
  main("agent_rewards.csv")
	
	


