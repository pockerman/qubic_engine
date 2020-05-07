# ```CubicEngine```

* [Dependencies](#dependencies)
* [Installation](#nstallation)
* [Examples Map](#examples_map)
    * [Filtering and Estimation](#filtering_and_esitmation)
    * [Control](#control)
    * [Machine Learning](#machine_learning)
    * [Reinforcement Learning](#reinforcement_learning)
    * [Robotics Simulations](#using_ros)
    * [Miscellaneous](#miscellaneous)

## <a name="dependencies"></a> Dependencies

- A C++17 compatible compiler
- <a href="https://www.boost.org/">Boost C++</a> libraries
- <a href="https://bitbucket.org/blaze-lib/blaze/wiki/browse/">Blaze</a> library for linear algebra
- <a href="https://github.com/google/googletest">GTest</a> if testing is enabled
- <a href="https://pytorch.org/">PyTorch</a> if PyTorch is enabled
- <a href="https://github.com/pockerman/compute_engine/tree/master/kernel">```kernel```</a>
- <a href="https://github.com/Neargye/magic_enum">```magic_enum```</a> only used if ```__GNUC__ >7 ```

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
- ```MAGIC_ENUM_INCL_DIR``` if ```magic_enum``` can be supported by the compiler (```__GNUC__ >7 ```)
```
mkdir build
cd build
cmake ..
make
make install
make tests
```

## <a name="examples_map"></a> Examples Map

### <a name="filtering_and_esitmation"></a> Filtering and Estimation

- <a href="#">Example 1: </a> Using the ```KalmanFilter``` class
- <a href="examples/exe2/doc/exe.ipynb">Example 2: </a> Using the ```ExtendedKalmanFilter``` class
- Unscented Kalman Filter
- Particle Filters

### <a name="control"></a> Control

- <a href="#">Example 4: </a> Using the ```PIDControl``` class
- LQR Control
- Model Predictive Control 

### <a name="machine_learning"></a> Machine Learning

- <a href="examples/exe5/doc/exe.md">Example 5: </a> Simulate standard error for the mean
- <a href="examples/exe7/doc/exe.md">Example 7: </a> Multithreaded batch gradient descent
- <a href="examples/exe11/doc/exe.md">Example 11: </a> Neural Network with PyTorch
- <a href="examples/exe12/doc/exe.md">Example 12: </a> Linear regression
- <a href="examples/exe13/doc/exe.md">Example 13: </a> Logistic regression classification
- <a href="examples/exe14/doc/exe.md">Example 14: </a> Logistic regression with ```GradientDescentWrapper```
- <a href="examples/exe15/doc/exe.md">Example 15: </a> Linear Regression with ```PYLinearRegressor```
- <a href="examples/exe16/doc/exe.md">Example 16: </a> Compare Lasso, Ridge and ElasticNet Regularizers
- <a href="examples/exe17/doc/exe.md">Example 17: </a> KMeans clustering
- <a href="examples/exe18/doc/exe.md">Example 18: </a> KNN classification
- <a href="examples/exe20/doc/exe.md">Example 20: </a> KNN classification with multiple threads

### <a name="reinforcement_learning"></a> Reinforcement Learning

- <a href="examples/exe6/doc/exe.md">Example 6: </a> Qlearning applied on simple ```GridWorld```
- <a href="examples/exe22/doc/exe.ipynb">Example 22: </a> SARSA learning applied on ```CliffWorld```

### <a name="using_ros"></a> Robotics Simulations

- <a href="#">Example 3: </a> Create a simple server to comminicate with ROS
- <a href="examples/exe19/doc/exe.md">Example 19</a> Differential drive system simulation
- <a href="examples/exe21/doc/exe.md">Example 21</a> Pure pursuit path tracking
- <a href="examples/exe23/doc/exe.md">Example 23</a> Follow the carrot path tracking


### <a name="miscellaneous"></a>Miscellaneous

- <a href="examples/exe9/doc/exe.md">Example 9: </a> A* search


