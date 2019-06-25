#!/usr/bin/python

import csv
from two_d_plotter import TwoDPlotter

try:

	filename = 'invalid_name' 
	plotType = input("Plot position (0), speed-time (1), heading-time (2), x-time (3), y-time (4), all(5) : ")

	csvfile = open(filename,'r')
	csvcontext = csv.reader(csvfile, delimiter=',')
	
	# 2D plot data
	x=[]
	y=[]

	if plotType == '0':

		plotter = TwoDPlotter(xlabel = "$X-coord$", ylabel="$Y-coord$")
	
		for row in csvcontext:
			
			if row[0].startswith('#'):
				continue
			x.append(float(row[0]))
			y.append(float(row[1]))
	
		plotter.plot(x, y)
		plotter.show_plots()

finally:

	if csvfile:
		csvfile.close()


