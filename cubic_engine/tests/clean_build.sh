echo "----->Removing everything in tests/build directory..."
rm -rf "build/"

echo "----->Creating tests/build directory..."
mkdir build

#now run fresh the build and test
./build_tests.sh
