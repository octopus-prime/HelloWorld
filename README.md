# HelloWorld

## Linux

cmake -DCMAKE_CXX_COMPILER=g++-13 -DCMAKE_BUILD_TYPE=Release -B build

## Windows

cmake -G "Visual Studio 17 2022" -A x64 -S . -B "build"

cmake --build build --config Release

## Performance (perft 7)

|       | This       | Crafty     | Stockfish  | QPerft     |
|-------|------------|------------|------------|------------|
| Nodes | 3195901860 | 3195901860 | 3195901860 | 3195901860 |
| Time  | 32.50      | 46.76      | 12.30      | 12.66      |
| Perf  | 98353479   | 68346917   | 259913944  | 252440905  |

## Crafty
perft 7

## Stockfish
stockfish bench 0 1 7 current perft

## QPerft
perft 7


## Master
n = 3195901860
c = 27690545
t = 32.50
p = 98353479

## Branch (Pseudo + Filter)
n = 3195901860
c = 6
t = 36.6345
p = 87237453

## Branch (Legal)
n = 3195901866 (+6)
c = 6
t = 13.744
p = 232531029
