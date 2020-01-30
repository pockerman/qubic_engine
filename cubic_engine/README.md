# ```CubicEngine```

* [Dependencies](#dependencies)
* [Installation](#nstallation)
* [How to use ](#how_to_use)
* [Some Issues](#some_issues)
* [Examples Map](#examples_map)
    * [Filtering and Estimation](#filtering_and_esitmation)
    * [Control](#control)
    * [Machine Learning](#machine_learning)
    * [Reinforcement Learning](#reinforcement_learning)
    * [Using ROS](#using_ros)

## <a name="dependencies"></a> Dependencies

- A C++17 compatible compiler
- <a href="https://www.boost.org/">Boost C++</a> libraries
- <a href="https://bitbucket.org/blaze-lib/blaze/wiki/browse/">Blaze</a> library for linear algebra
- <a href="https://github.com/google/googletest">GTest</a> if testing is enabled
- <a href="https://pytorch.org/">PyTorch</a> if PyTorch is enabled
- <a href="https://github.com/pockerman/compute_engine/tree/master/kernel">```kernel```</a>

## <a name="nstallation"></a> Installation

Currently you need to tweak the supplied ```CMakeLists.txt```. Concretely, you will have to
specify the following:

- The ```BLAZE_INCL_DIR``` variable
- ```ENABLE_TESTING``` by default is set to ```ON``` and in this case you need to specify
    - ```GTEST_INC_DIR```
    - ```GTEST_LIB_DIR```
- The compilation mode ```Debug``` or ```Release```. Default is ```Debug```
- ```USE_OPENMP``` by default is ```ON``` meaning use OpenMP specification
- ```USE_PYTORCH``` by default is ```OFF``` meaning do no use PyTorch C++ front end 
		- If you choose to set ```USE_PYTORCH``` to ON you also need to set properly the flag ```CMAKE_PREFIX_PATH```; by default is set to ```${PWD}../libtorch``` 

- ```CMAKE_INSTALL_PREFIX``` by default is set to ```${PWD}/install```
- ```KERNEL_INCL_DIR``` and ```KERNEL_LIB_DIR``` by default these point to ```${PWD}../kernel/include``` and  ```${PWD}../kernel/install/lib``` respectively

```
mkdir build
cd build
cmake ..
make
make install
make tests
```

## <a name="dependencies"></a> How to use

Checkout the [Examples Map](#examples_map). Documentation is well...underway.

## <a name="some_issues"></a> Some Issues

```
undefined reference to `dgetri_'
```

This means that you need to link your executable to BLAS for example adding something like
the following should resolve the issue

```
TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)
```

## <a name="examples_map"></a> Examples Map

### <a name="filtering_and_esitmation"></a> Filtering and Estimation

- <a href="#">Example 1</a> Using the ```KalmanFilter``` class
- <a href="#">Example 2</a> Using the ```ExtendedKalmanFilter``` class
- <a href="#">Unscented Kalman Filter</a>
- <a href="#">Particle Filters</a>
- <a href="#">Ordinary Least Squares</a>

### <a name="control"></a> Control

- <a href="#">Example 4</a> Using the ```PIDControl``` class
- LQR Control

### <a name="machine_learning"></a> Machine Learning

- <a href="examples/exe5/doc/exe.md">Example 5</a> Simulate standard error for the mean
- <a href="examples/exe7/doc/exe.md">Example 7</a> Multithreaded batch gradient descent
- <a href="examples/exe11/doc/exe.md">Example 11</a> Neural Network with PyTorch
- <a href="examples/exe12/doc/exe.md">Example 12</a> Linear regression
- <a href="examples/exe13/doc/exe.md">Example 13</a> Logistic regression classification
- <a href="examples/exe14/doc/exe.md">Example 14</a> Logistic regression with ```BatchGradientDescentWrapper```
- <a href="examples/exe16/doc/exe.md">Example 16</a> Compare Lasso, Ridge and ElasticNet Regularizers
- <a href="examples/exe15/doc/exe.md">Example 15</a> Linear Regression With ```PYLinearRegressor```
- <a href="examples/exe17/doc/exe.md">Example 17</a> KMeans clustering
- <a href="examples/exe18/doc/exe.md">Example 18</a> KNN classification

### <a name="reinforcement_learning"></a> Reinforcement Learning

- <a href="#">Example 6</a> Solving the State-Value Function Problem

### <a name="using_ros"></a> Using ROS

- <a href="#">Example 3</a> Create a simple server to comminicate with ROS


