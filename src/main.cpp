#include "position.hpp"
#include "test.hpp"
#include <chrono>
#include <iostream>

void run() {
//  const position position{"r1b1k1n1/5pr1/n1p5/1pbPp1q1/Pp5P/N1PPBP1B/8/2RQK2R w K -"};
//  position.printt(3);
//  const test t{"r4br1/8/2Q2npp/Pkn1p3/8/2PPP1qP/4bP2/RNB1KB2 b Q -,1,18,888,16244,779306,14947584"};
//  std::cout << "N = " << t.run() << std::endl;


//  const position position;
//  const position position{"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"};
//  const position position{"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -"};
//  constexpr auto depth = 8;
//  position.printt(depth);

//  for (int i = 0; i <= depth; ++i) {
    auto time0 = std::chrono::high_resolution_clock::now();
//    auto count = position.perft(i);
//    position.printt(i);
//    auto count = 0;
    auto count = test::run("epd/perft_long.txt");
    auto time1 = std::chrono::high_resolution_clock::now();
    const auto time =
        std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>(
            time1 - time0);

//    std::cout << "d = " << i << std::endl;
    std::cout << "n = " << count << std::endl;
    std::cout << "t = " << time.count() << std::endl;
    std::cout << "p = " << size_t(count / time.count()) << std::endl;
    std::cout << std::endl;
//  }
}

int main() {
    try {
        run();
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
}
