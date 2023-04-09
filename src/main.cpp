#include "position.hpp"
#include "test.hpp"
#include <chrono>
#include <iostream>

// n = 135610529153
// t = 121.005
// p = 1120704947
auto todo1 = []() -> size_t { return test::run("epd/perft_long.txt", 5); };

// n = 3195901860
// t = 34.6161
// p = 92324105
auto todo2 = []() { return position{}.perft2(7); };

auto todo3 = []() -> size_t { position{"4k3/6N1/5b2/4R3/8/8/8/4K3 b - - 0 1"}.printt(1); return 0; };
auto todo4 = []() -> size_t { position{"4k3/8/5b2/4R3/8/8/8/4K3 b - - 0 1"}.printt(1); return 0; };
auto todo5 = []() -> size_t { position{"4k3/8/6n1/4R3/8/8/8/4K3 b - - 0 1"}.printt(1); return 0; };
auto todo6 = []() -> size_t { position{"8/8/8/1k6/3Pp3/8/8/4KQ2 b - d3 0 1"}.printt(1); return 0; };
auto todo7 = []() -> size_t { position{"8/8/8/2k5/3Pp3/8/8/4K3 b - d3 0 1"}.printt(1); return 0; };

void run() {
  using as_floating_point = std::chrono::duration<double, std::ratio<1>>;

  auto time0 = std::chrono::high_resolution_clock::now();
  auto [count,checks] = todo2();
//  auto count = todo7();
//  auto checks = 0;
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
