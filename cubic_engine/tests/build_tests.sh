cd build
echo "----->Start configuring tests..."
echo " "
cmake -D CMAKE_CXX_COMPILER=/usr/lib/ccache/g++ ..

echo " "

echo "----->Start building tests..."

echo " "

make

echo " "

echo "----->Start running tests..."
make test
