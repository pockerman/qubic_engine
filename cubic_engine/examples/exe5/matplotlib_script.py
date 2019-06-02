#!/usr/bin/python
import csv
import numpy as np
import matplotlib.pyplot as plt

def string_to_float(s):
	try:
		return float(s)
	except ValueError:
		return -1.0;

csvfile = open('Positions.csv','rb')
csvcontext = csv.reader(csvfile,delimiter=',')

x=[]
y=[]

for row in csvcontext:
	if row[0][0] != '#':
		x.append(string_to_float(row[0]))
		y.append(string_to_float(row[1]))

fig = plt.figure()

ax1 = fig.add_subplot(111)
ax1.set_title("X-Y Robot Position")
ax1.set_xlabel("X-position")
ax1.set_ylabel("Y-position")
ax1.plot(x,y, marker='o', c='r', linestyle=' ',label='')

leg = ax1.legend()
plt.show()
