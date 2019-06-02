#!/usr/bin/python

import csv
import numpy as np
import matplotlib.pyplot as plt

def string_to_float(s):
	try:
		return float(s)
	except ValueError:
		print "Cannot convert string to float"

def plot_one_var(plot_title,x_title,y_title,x,y,xmesh,ymesh):

	fig = plt.figure()
	ax1 = fig.add_subplot(111)
	ax1.set_title(plot_title)
	ax1.set_xlabel(x_title)
	ax1.set_ylabel(y_title)
	ax1.plot(x,y, marker='o', c='r', linestyle=' ',label='')
	ax1.plot(xmesh,ymesh, marker='*', c='b',linestyle='',label='')

	

	leg = ax1.legend()
	plt.show()

time_idx = 0
speed_idx = 1
heading_idx =2
x_idx = 3
y_idx = 4
z_idx = 5

plotAll = False
plotPosition = False
plotSpeed = False
plotHeading = False

try:

	filename = 'Positions.csv' #input("Eneter solution file : ")
	plotType = input("Plot position (0), speed-time (1), heading-time (2), x-time (3), y-time (4), all(5) : ")

	csvfile = open(filename,'rb')
	csvcontext = csv.reader(csvfile,delimiter=',')

	if plotType == '0':

		# coordinates of the position
		x=[]
 		y=[]

		meshfilename = 'world.csv'
		csvfilem = open(meshfilename,'rb')
		csvcontextm = csv.reader(csvfilem,delimiter=',')

		for row in csvcontext:
			x.append(string_to_float(row[0]))
			y.append(string_to_float(row[1]))

		xmesh = []
		ymesh = []

		for row in csvcontextm:
			xmesh.append(string_to_float(row[0]))
			ymesh.append(string_to_float(row[1]))

		if csvfilem:
			csvfilem.close()

		plot_one_var("AC NED position history","x-coord","y-coord",x,y,xmesh,ymesh)
		#plot_one_var("Start/Goal position","x-coord","y-coord",x,y,xmesh,ymesh)

	elif plotType == '1':

		#coordinates of speed and time
		x = []
 		y = []

		for row in csvcontext:
			x.append(string_to_float(row[time_idx]))
			y.append(string_to_float(row[speed_idx]))

		plot_one_var("AC Speed history","Time (sec) ","Speed (m/s)",x,y)

	elif plotType == '2':

		x = []
 		y = []

		for row in csvcontext:
			x.append(string_to_float(row[time_idx]))
			y.append(string_to_float(row[heading_idx]))

		plot_one_var("AC Heading history","Time (sec) ","Heading ",x,y)

	elif plotType == '3':

		x = []
 		y = []

		for row in csvcontext:
			x.append(string_to_float(row[time_idx]))
			y.append(string_to_float(row[x_idx]))

		plot_one_var("AC x-position history","Time (sec) ","X",x,y)

	elif plotType == '4':

		x = []
 		y = []

		for row in csvcontext:
			x.append(string_to_float(row[time_idx]))
			y.append(string_to_float(row[y_idx]))

		plot_one_var("AC y-position history","Time (sec) ","Y",x,y)


finally:

	if csvfile:
		csvfile.close()


