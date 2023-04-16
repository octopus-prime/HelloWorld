#include "position.hpp"
#include "test.hpp"
#include <chrono>
#include <iostream>

// n = 135610529153
// t = 121.005
// p = 1120704947
auto todo1 = []() -> size_t { return test::run("epd/perft_long.txt", 3); };

// n = 3195901860
// t = 34.6161
// p = 92324105
auto todo2 = []() { return position{}.perft_legal(7); };

auto todo2a = []() { position{}.printt(1); return 0; };

auto todo3 = []() -> size_t { position{"4k3/6N1/5b2/4R3/8/8/8/4K3 b - - 0 1"}.printt(3); return 0; };
auto todo3broken = []() -> size_t { position{"8/5kN1/5b2/5R2/8/8/8/4K3 b - - 0 1"}.printt(1); return 0; };
auto todo3a = []() -> size_t { return position{"4k3/6N1/5b2/4R3/8/8/8/4K3 b - - 0 1"}.perft_legal(8); };
auto todo4 = []() -> size_t { position{"4k3/8/5b2/4R3/8/8/8/4K3 b - - 0 1"}.printt(1); return 0; };
auto todo5 = []() -> size_t { position{"4k3/8/6n1/4R3/8/8/8/4K3 b - - 0 1"}.printt(1); return 0; };
auto todo6 = []() -> size_t { position{"8/8/8/1k6/3Pp3/8/8/4KQ2 b - d3 0 1"}.printt(1); return 0; };
auto todo7 = []() -> size_t { position{"8/8/8/2k5/3Pp3/8/8/4K3 b - d3 0 1"}.printt(1); return 0; };
auto todo8 = []() -> size_t { position{"4k3/8/4r3/8/8/4Q3/8/2K5 b - - 0 1"}.printt(1); return 0; };
auto todo9 = []() -> size_t { position{"R2rk1rR/5b2/8/4q2B/8/4Q3/1B6/2K5 b - - 0 1"}.printt(1); return 0; };
auto todo10 = []() -> size_t { position{"4k3/5n2/8/7B/8/8/8/4K3 b - - 0 1"}.printt(1); return 0; };
auto todo11 = []() -> size_t { position{"8/8/2Rb1k2/8/3p4/6pb/1B4P1/4K3 w - - 0 1"}.printt(2); return 0; };
auto todo12 = []() -> size_t { position{"8/8/2Rb1k2/8/3p4/6pb/1B4P1/3K4 b - - 0 1"}.printt(1); return 0; };
auto todo13 = []() -> size_t { position{"3rkb2/4pprp/2P1nnp1/p7/pP6/2P4b/3B1P1P/1RNK1BqR w - - 0 1"}.printt(1); return 0; };

void run() {
  using as_floating_point = std::chrono::duration<double, std::ratio<1>>;

  auto time0 = std::chrono::high_resolution_clock::now();
//  auto [count,checks] = todo2();
  auto count = todo2();
  auto checks = 0;
  auto time1 = std::chrono::high_resolution_clock::now();
  auto time = duration_cast<as_floating_point>(time1 - time0).count();

  std::cout << "n = " << count << std::endl;
  std::cout << "c = " << checks << std::endl;
  std::cout << "t = " << time << std::endl;
  std::cout << "p = " << size_t(count / time) << std::endl;
  std::cout << std::endl;
}

int main() {
  try {
    run();
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl;
  }
}
