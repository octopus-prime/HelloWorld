# HelloWorld

cmake -DCMAKE_CXX_COMPILER=g++-13 -DCMAKE_BUILD_TYPE=Release -B build


cmake -G "Visual Studio 17 2022" -A x64 -S . -B "build"
cmake --build build --config Release