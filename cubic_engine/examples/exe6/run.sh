#!/bin/bash
if [ -f exe ] ; then 
printf "removing executable exe" 
rm exe 
echo ""
fi 
cmake -D CMAKE_CXX_FLAGS=-std=c++14 -g CMAKE_CXX_COMPILER=/usr/lib/ccache/g++ .
make 
if [ -f exe ] ; then 
printf "================================================" 
echo "" 
printf "Running Example " 
echo "" 
printf "================================================"
echo "" 
echo "" 
./exe 
echo "" 
echo "" 
printf "================================================"
echo "" 
printf "Done Running Example "
echo "" 
printf "================================================"
echo "" 
else 
printf "Executable exe was not found"
echo "" 
fi 
