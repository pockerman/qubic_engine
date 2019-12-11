# Compute Engine

## Dependencies



## Installation

## Some Issues


```
undefined reference to `dgetri_'
```

This means that you need to link your executable to BLAS for example adding something like
the following should resolve the issue

```
TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)

``` 
