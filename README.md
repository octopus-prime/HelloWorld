# HelloWorld

## Linux

cmake -DCMAKE_CXX_COMPILER=g++-13 -DCMAKE_BUILD_TYPE=Release -B build

## Windows

cmake -G "Visual Studio 17 2022" -A x64 -S . -B "build"

cmake --build build --config Release

## Performance (perft 7)

|       | This       | Crafty     | Stockfish        |
|-------|------------|------------|------------------|
| Nodes | 3195901860 | 3195901860 | 3195901860       |
| Time  | 34.62      | 46.76      | 12.30            |
| Perf  | 92324105   | 68346917   | 259913944 (wtf!) |


## Crafty
perft 7

## Stockfish
stockfish bench 0 1 7 current perft
