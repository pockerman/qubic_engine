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
		
    pathx = [0., 10.0, 20., 50., 60., 90., ]
    pathy = [0.0, 0.0, 0.0, 30., 30., 0.0]

    look_ahead_x = []
    look_ahead_y = []

    
    for row in csv_file_reader:
		
      if row_count > 1:
        statex.append(float(row[0]))
        statey.append(float(row[1]))
        #look_ahead_x.append(float(row[3]))
        #look_ahead_y.append(float(row[4]))
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
      legend.append("Lookahead Points")
      
    plt.xlabel('x')
    plt.ylabel('y')
    plt.legend(legend)

    plt.axis('square')
    plt.tight_layout(pad=0)
    plt.plot()
    plt.show()
	
if __name__ == '__main__':
  main("agent_state.csv")

	


