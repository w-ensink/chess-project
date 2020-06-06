
mkdir build && cd build
mkdir cmake && cd cmake
cmake ../..
cmake --build .
mv src/chess-engine ../chess-engine
cd .. && rm -rf cmake
