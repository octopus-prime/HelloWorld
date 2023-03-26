#include "position.hpp"
#include <chrono>
#include <iostream>

int main() {
  const position position;
//  const position position{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};
//  const position position{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -"};
  constexpr auto depth = 8;
//  position.printt(depth);

  for (int i = 0; i <= depth; ++i) {
    auto time0 = std::chrono::high_resolution_clock::now();
    auto count = position.perft(i);
//    position.printt(i);
//    auto count = 0;
    auto time1 = std::chrono::high_resolution_clock::now();
    const auto time =
        std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>(
            time1 - time0);

    std::cout << "d = " << i << std::endl;
    std::cout << "n = " << count << std::endl;
    std::cout << "t = " << time.count() << std::endl;
    std::cout << "p = " << size_t(count / time.count()) << std::endl;
    std::cout << std::endl;
  }
}
