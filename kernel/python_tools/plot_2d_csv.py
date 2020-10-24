#!/usr/bin/python

import csv
from two_d_plotter import TwoDPlotter

try:

	filename = '/home/david/CubicEngineering/cubic_engine/build-cubic_engine-Desktop_Qt_5_12_1_GCC_64bit-Default/examples/exe2/robot_state.csv' 
	csvfile = open(filename,'r')
	csvcontext = csv.reader(csvfile, delimiter=',')
	
	# 2D plot data
	x=[]
	y=[]
	x_true=[]
	y_true=[]
	zx_true=[]
	zy_true=[]

	plotter = TwoDPlotter(plot_title='EKF Estimated vs True 2D position', xlabel = "$X-$coordinate", ylabel="$Y-$coordinate")
	
	for row in csvcontext:
		
		if row[0].startswith('#'):
			continue
			
		x.append(float(row[0]))
		y.append(float(row[1]))
		x_true.append(float(row[2]))
		y_true.append(float(row[3]))
		#zx_true.append(float(row[6]))
		#zy_true.append(float(row[7]))
	
	plotter.plot(x, y, label='Estimated X-Y')
	plotter.plot(x_true, y_true, label='True X-Y')
	#plotter.plot(zx_true, zy_true)
	plotter.show_plots(show_legend=True)

finally:

	if csvfile:
		csvfile.close()


