mkdir build
cd build
mkdir debug
cd debug
cmake -DUSE_GCOV=yes ../.. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=$1 -DCMAKE_CROSSCOMPILING=1 -DRUN_HAVE_STD_REGEX=0 -DRUN_HAVE_POSIX_REGEX=0
make
./eswitch_tests
make coverage