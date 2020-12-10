rm -rf build/release

mkdir build
cd build
mkdir release
cd release
cmake ../.. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=$1
make
