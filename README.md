# Compute Engine

This is my C++ playground.

## Dependencies

- A C++17 compatible compiler
- <a href="https://www.boost.org/">Boost C++</a> libraries
- <a href="https://bitbucket.org/blaze-lib/blaze/wiki/browse/">Blaze</a> library for linear algebra 
- <a href="https://github.com/google/googletest">GTest</a> if testing is enabled: 


## Installation

Currently you need to tweak the supplied ```CMakeLists.txt``` supplied. Concretely, you will have to
specify the following:

- The ```BLAZE_INCL_DIR``` variable
- ```ENABLE_TESTING``` by default is set to ```ON``` and in this case you need to specify
    - ```GTEST_INC_DIR```
    - ```GTEST_LIB_DIR```
- The compilation mode ```Debug``` or ```Release```. Default is ```Debug```
- ```USE_OPENMP``` by default is ```ON``` meaning use OpenMP specification

If you have any problems checkout the section Some Issues below

## How to use

```kernel``` and ```CubicEngine``` have some examples to get you started. Documentation is well...underway.

## Some Issues

```
undefined reference to `dgetri_'
```

This means that you need to link your executable to BLAS for example adding something like
the following should resolve the issue

```
TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)

``` 
