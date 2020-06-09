
# remove previous build, if it exists
rm -rf build

# make the build dir and change into it
mkdir build && cd build

# make dir to temporarily store cmake files
mkdir cmake && cd cmake

# build target
cmake ../.. && cmake --build .

# move binary to build directory
mv src/chess-engine ../chess-engine

# remove temporary cmake files
cd .. && rm -rf cmake
