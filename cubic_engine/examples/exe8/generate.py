import numpy as np
import math
import scipy.stats
import matplotlib.pyplot as plt
#from scipy.stats import norm

#global constants
PI = 3.14159265359
NUM_STATES = 4

#samples 
N = 500;

#time step size
T = 0.2;

#Parameters
m = 1000;
b = 0.35;
p = 3.92;
g = 9.8;

#noises
bt = 0.01;      
bd = 50;        
w1 = 0.01;
w2 = 0.01;

def model(x_k_1,F):
	xk = np.zeros(4)
	xk[0] = x_k_1[0] + x_k_1[1]*T + T**2/2*(F/m - b/m*x_k_1[1]**2)
	xk[1] = x_k_1[1] + (F/m - b/m*x_k_1[1]**2)*T + scipy.stats.norm.pdf(x_k_1[1],loc=0,scale=w1) 
	xk[2] = x_k_1[2] + x_k_1[3]*T + T**2/2*(p/m*x_k_1[1]**2 - g) 
	xk[3] = x_k_1[3] + (p/m*x_k_1[1]**2 - g)*T + scipy.stats.norm.pdf(x_k_1[3],loc=0,scale = w2)
	return xk

#generate time
time = np.zeros(N)
for t in range(N):
	time[t] = t*T
	
#generation of the plane thrust vector
F = np.zeros(N) 

for t in range(N):
	F[t] = 875 +5.*math.sin(t/N*2*2*PI)

#initial conditions
x0=np.zeros(NUM_STATES)
x0[0] = -2500.
x0[1] = 50.
x0[2] = 250.
x0[3] = 0.

#Simulation and generation of the real plane trajectory
trajectory = np.zeros((N,4))
trajectory[0] = x0;

#print(trajectory)
for t in range(N):
	if t > 1:
		trajectory[t] = model(trajectory[t-1],F[t])

print(trajectory)

#extract the x position of the model
xpos = np.zeros(N)
for i in range(N):
	xpos[i] = trajectory[i][0]

fig, ax = plt.subplots(1, 1)
ax.plot(time,xpos,'r-', lw=5, alpha=0.6, label='norm pdf')
plt.show()

