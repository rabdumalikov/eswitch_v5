rm -rf build/release

mkdir build
cd build
mkdir release
cd release
cmake ../.. -DCMAKE_BUILD_TYPE=Release
make
