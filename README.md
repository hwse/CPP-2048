# Build Requirements

* Conan
* CMake
* C++ Compiler

The bincrafters remote must be added to conan:

    conan remote list
    conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan

# Build Steps

    mkdir build
    cd build

    conan install .. -s build_type=Debug
    cmake ..
    cmake --build .