# Install Trilinos

This guide goes over the Trilinos installation process

* [Download Trilinos](#dependencies)
* [Installation](#installation)


## <a href="dependencies"></a> Download Trilinos

You can download the Trilinos project from here: <a href="https://github.com/trilinos/Trilinos">here</a>.

## <a href="installation"></a> Installation

```cd``` to the Trilinos repo you downloaded above. Create a ```build``` directory and ```cd build```.
Trilinos is a large collection of packages however ```kernellib``` only uses a very small fraction  
of it. Trilinos requires to explicitly decalre which packages you want to install. The script below
installs ```Epetra```, ```AztecOO``` and ```Ifpack```. Copy the script and execute it in the
```build``` directory you created above. This is should create the necessary build files, build the 
needed packages and their dependencies and install Trilinos at the location
specified by the  ```DCMAKE_INSTALL_PREFIX``` variable. 
Note that this configuration does not enbale MPI support for Trilinos. 

```
#!/bin/bash
rm CMakeCache.txt
cmake \
 -DTrilinos_ENABLE_EXPLICIT_INSTANTIATION=ON \
 -DBUILD_SHARED_LIBS=ON \
 -DTrilinos_ENABLE_Epetra=ON \
 -DTrilinos_ENABLE_AztecOO=ON \
 -DTrilinos_ENABLE_Ifpack=ON \
 -DTrilinos_ENABLE_Amesos2=ON \
-DTrilinos_ENABLE_Amesos=ON \
 -DCMAKE_INSTALL_PREFIX=<path-to-install-trilinos> \
 ..
  
make 
make install 
```

To enable MPI support you must minimally set:

```
-DTPL_ENABLE_MPI=ON
```






