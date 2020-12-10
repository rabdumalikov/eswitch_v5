rm -rf build/debug

mkdir build
cd build
mkdir debug
cd debug
cmake -DUSE_GCOV=yes ../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$1
make
