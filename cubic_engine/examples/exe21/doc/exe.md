# Example 21: Pure Pursuit Path Tracking

## Contents

* [Overview](#overview) 
	* [Pure pursuit path tracking](#pure_pursuit_tracking)
* [Include files](#include_files)
* [Program structure](#prg_struct)
* [The main function](#m_func)
* [Results](#results)
* [Source Code](#source_code)


## <a name="overview"></a> Overview

In this example we will implement the socalled pure pursuit path tracking algorithm.
For more details you can consult the original document <a href="https://www.ri.cmu.edu/pub_files/pub3/coulter_r_craig_1992_1/coulter_r_craig_1992_1.pdf">here</a>.

### <a name="pure_pursuit_tracking"></a> Pure pursuit path tracking

Pure pursuit path tracking is a geometry based path following controller.
It uses the current position of the robot along sidea selected point on the path to make adjustements
if needed in the position and orientation of the vehicle such that the latter abides with following
the specified path as closely as possible. This point is chosen as a specified look-ahead distacnce.



The workings of the alogorithm are as follows:

- Determine the current location of the vehicle
- Find the path point closest to the vehicle
- Find the lookahead point
- Transform the look ahead to vehicle coordinates
- Calculate the curvature and request the vehicle to set the steering to curvature
- Update the vehicle's position

The major advantages of the algorithm can be summarized as follows

- Easy tuning of the look-ahead distance
- Computational simplicity
- Absence of derivative terms

There are also some disadvantages


## <a name="include_files"></a> Include files

```
```

## <a name="m_func"></a> The main function

```

```

## <a name="results"></a> Results

<img src="movie.gif"
     alt="Position view"
     style="float: left; margin-right: 10px;" />

## <a name="source_code"></a> Source Code

<a href="../exe.cpp">exc.cpp</a>

