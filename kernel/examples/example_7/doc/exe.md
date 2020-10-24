# Example: Longitudinal Vehicle Model

## Contents
* [Acknowledgements](#ackw)
* [Overview](#overview) 
                * [Motion Model](#motion_model)
                * [PID Control](#pid_control)
* [Include files](#include_files)
* [Program structure](#prg_struct)
				* [The Dynamics Class](#dynamics_class)
				* [The PIDController class](#pid_control_struct)
* [The main function](#m_func)
* [Results](#results)

## <a name="ackw"></a>  Acknowledgements

This example has been taken from Coursera's course <a href="https://www.coursera.org/specializations/self-driving-cars">Introduction to Self-Driving Cars</a>

## <a name="overview"></a> Overview

In this example, we will implement the forward longitudinal vehicle model. 
The model accepts throttle inputs and steps through the longitudinal dynamic equations. 
Once implemented, we will use a set of inputs that drive the vehicle over a small road slope in order to  test our model.

The input to the model is a throttle percentage \f$ x_{\theta} \in [0,1] $\f
which provides torque to the engine and subsequently accelerates the vehicle for forward motion.

The dynamic equations consist of many stages to convert throttle inputs to wheel speed (engine -> torque converter -> transmission -> wheel). 
These stages are bundled together in a single inertia term  \f$ J_e $\f which is used in the following combined engine dynamic equations.

\f[
 J_e\dot{\omega}_e = T_e - GRr_{eff}F_{load}
\f]

\f[
 m\ddot{x} = F_x - F_{load}
\f]

Where \f$ T_e $\f is the engine torque, \f$ GR $\f is the gear ratio, \f$ r_{eff} $\f is the effective radius, 
\f$ m $\f is the vehicle mass, \f$ x $\f is the vehicle position, 
\f$ F_x $\f is the tire force, and \f$ F_{load} $\f is the total load force.

The engine torque, \f$ T_e $\f,  is computed from the throttle input \f$ x_{\theta} $\f 
and the engine angular velocity \f$ \omega_e $\f using a simplified quadratic model.

\f[
 T_e = x_{\theta}(\alpha_0 + \alpha_1 \omega_e +\alpha_2 \omega_{e}^2)
\f] 

The load forces consist of aerodynamic drag \f$ F_{aero} $\f, rolling friction \f$ R_x $\f, and 
gravitational force \f$F_g $\f from an incline at angle \f$ \alpha $\f. The aerodynamic 
drag is a quadratic model and the friction is a linear model.



Where ωw
is the wheel angular velocity and s

is the slip ratio.

We setup the longitudinal model inside a Python class below. 
The vehicle begins with an initial velocity of 5 m/s and engine speed of 100 rad/s. All the relevant parameters are defined and like the bicycle model, a sampling time of 10ms is used for numerical integration.

Using the model, you can send constant throttle inputs to the vehicle in the cell below. You will observe that the velocity converges to a fixed value based on the throttle input due to the aerodynamic drag and tire force limit. A similar velocity profile can be seen by setting a negative incline angle αα. In this case, gravity accelerates the vehicle to a terminal velocity where it is balanced by the drag force.


### <a name="motion_model"></a> Motion Model

###<a name="pid_control"></a> PID Control

## <a name="include_files"></a> Include files

## <a name="prg_struct"></a> Program structure

## <a name="m_func"></a> The main function

## <a name="results"></a> Results




