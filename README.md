# Compute Engine

This is my C++ playground

## Dependencies

- A C++17 compatible compiler
- <a href="https://www.boost.org/">Boost C++</a> libraries
- <a href="https://bitbucket.org/blaze-lib/blaze/wiki/browse/">Blaze</a> library for linear algebra 
- <a href="https://github.com/google/googletest">GTest</a> if testing is enabled: 


## Installation

Currently you need to tweak the supplied ```CMakeLists.txt``` supplied

## Some Issues

```
undefined reference to `dgetri_'
```

This means that you need to link your executable to BLAS for example adding something like
the following should resolve the issue

```
TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)

``` 
