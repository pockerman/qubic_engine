"""
Utility script for plotting with matplotlib
"""
import matplotlib.pyplot as plt
import csv

def main(filename):
	
  with open(filename, 'r', newline='') as csvfile:
    csv_file_reader = csv.reader(csvfile, delimiter=",")
		
    row_count = 0
    statex = []
    statey = []
		
    pathx = [0.5,
1.5,
2.5,
3.5,
3.5,
3.5,
3.5,
4.5,
4.5,
5.5,
5.5,
6.5,
7.5,
7.5,
7.5,
7.5,
8.5,
9.5,
9.5,]
    
    
    pathy = [0.5,
0.5,
0.5,
0.5,
1.5,
2.5,
3.5,
3.5,
4.5,
4.5,
5.5,
5.5,
5.5,
6.5,
7.5,
8.5,
8.5,
8.5,
9.5,]

    look_ahead_x = []
    look_ahead_y = []

    #closest_x = []
    #closest_y = []
		
    for row in csv_file_reader:
		
      if row_count > 1:
        statex.append(float(row[0]))
        statey.append(float(row[1]))
        look_ahead_x.append(float(row[3]))
        look_ahead_y.append(float(row[4]))
        #closest_x.append(float(row[5]))
        #closest_y.append(float(row[6]))
      row_count += 1
		
    legend = []
    if len(statex) != 0:
      plt.plot(statex, statey, 'b-')
      legend.append("Vehicle position")
		
    if len(pathx) != 0:
      plt.plot(pathx, pathy, 'ro-')
      legend.append("Path")

    if len(look_ahead_x) != 0:
      plt.plot(look_ahead_x, look_ahead_y, 'go')
      #plt.plot(closest_x, closest_y, 'yo')
      legend.append("Lookahead Points")
      #legend.append("Closest Points")
			
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

	


