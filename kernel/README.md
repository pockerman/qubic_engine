# ```kernel```

* [Dependencies](#dependencies)
* [Installation](#nstallation)
* [How to use ](#how_to_use)
* [Some Issues](#some_issues)
* [Examples Map](#examples_map)
    * [Multi-threading](#multi_threading)
    * [Linear Algebra](#linear_algebra)
    * [Numerics](#numerics)
    * [Physics Simulation](#physics_simulation)
    * [Miscellaneous](#miscellaneous)


## <a name="dependencies"></a> Dependencies

- A C++17 compatible compiler
- <a href="https://www.boost.org/">Boost C++</a> libraries
- <a href="https://bitbucket.org/blaze-lib/blaze/wiki/browse/">Blaze</a> library for linear algebra
- <a href="https://github.com/google/googletest">GTest</a> if testing is enabled
- <a href="https://github.com/trilinos/Trilinos">Trilinos</a> if it is enabled.

## <a name="nstallation"></a> Installation

Currently you need to tweak the supplied ```CMakeLists.txt```. Concretely, you will have to
specify the following:

- The ```BLAZE_INCL_DIR``` variable
- ```ENABLE_TESTING``` by default is set to ```ON``` and in this case you need to specify
    - ```GTEST_INC_DIR```
    - ```GTEST_LIB_DIR```
- The compilation mode ```Debug``` or ```Release```. Default is ```Debug```
- ```USE_OPENMP``` by default is ```ON``` meaning use OpenMP specification
- ```USE_LOG``` by default is ```ON``` meaning use logging
- ```USE_TRILINOS``` by default is ```OFF``` meaning do not use Trilinos. If the variable is set to ```ON``` then the following
variables should also be set ```TRILINOS_INCL_DIR``` and ```TRILINOS_LIB_DIR```

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

## Some Issues

```
undefined reference to `dgetri_'
```

This means that you need to link your executable to BLAS for example adding something like
the following should resolve the issue

```
TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)

```

## <a name="examples_map"></a> Examples Map

### <a name="multi_threading"></a> Multi-threading

- <a href="examples/example_1/doc/exe.md">Example 1: </a> Create ```SimpleTask```s
- <a href="examples/example_2/doc/exe.md">Example 2: </a> Create  ```StoppableTask```s
- <a href="examples/example_5/doc/exe.md">Example 5: </a> Parallel Sum Array Elements
- <a href="examples/example_9">Example 9: </a> ```parallel_for``` pattern with ```ThreadPool``` class
- <a href="examples/example_12">Example 12: </a> Producer-Consumer pattern
- <a href="examples/example_13">Example 13: </a> ```parallel_for``` pattern with ```OMPExecutor``` class

### <a name="linear_algebra"></a> Linear Algebra

- <a href="examples/example_4/doc/exe.md">Example 4: </a> Multithreaded Jacobi Iterator
- <a href="examples/example_6">Example 6: </a> Multithreaded Conjugate Gradient solver


### <a name="numerics"></a> Numerics

- <a href="examples/example_8">Example 8: </a> Serial Monte Carlo Integration
- <a href="examples/example_10">Example 10: </a> Calculate the area of a circle using Monte Carlo integration
- <a href="examples/example_11">Example 11: </a> Radio active deacay simulation
- <a href="examples/example_14">Example 14: </a> Monte Carlo Integration with ```OMPExecutor``` class
- <a href="examples/example_15/doc/exe.md">Example 15: </a> Calculate MSE error with ```MSEFunction``` class
- <a href="examples/example_16/doc/exe.md">Example 16: </a> Calculate SSE error with ```SSEFunction``` class


### <a name="physics_simulation"></a> Physics Simulation

- <a href="#">Example 7: </a> Longitudinal Vehicle Model
- <a href="examples/example_17/doc/exe.md">Example 17: </a>Simulate differential drive system
- <a href="examples/example_19/doc/exe.md">Example 19: </a>Solve Laplace equation with collocated  FVM
- <a href="examples/example_20/doc/exe.md">Example 20: </a>Solve Laplace equation with collocated  FVM and multiple threads
- <a href="examples/example_21/doc/exe.md">Example 21: </a>Solve Convection Equation With FVM

### <a name="miscellaneous"></a> Miscellaneous

- <a href="examples/example_18/doc/exe.md">Example 18: </a> Create a rectangular grid
