#!/usr/bin/python

import csv
import numpy as np
import matplotlib.pyplot as plt

def string_to_float(s):
	try:
		return float(s)
	except ValueError:
		return -1.0;

csvfile = open('robot_state.csv','rt')
csvcontext = csv.reader(csvfile,delimiter=',')
X=[]
Y=[]

for row in csvcontext:
	X.append(string_to_float(row[0]))
	Y.append(string_to_float(row[1]))
	
fig = plt.figure()
plt.plot(X,Y,'ro-')
plt.xlabel('X-coordinate (mm)')
plt.ylabel('Y-coordinate (mm)')
plt.show()
