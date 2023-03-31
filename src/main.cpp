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
auto todo2 = []() -> size_t { return position{}.perft(7); };

void run() {
  using as_floating_point = std::chrono::duration<double, std::ratio<1>>;

  auto time0 = std::chrono::high_resolution_clock::now();
  auto count = todo2();
  auto time1 = std::chrono::high_resolution_clock::now();
  auto time = duration_cast<as_floating_point>(time1 - time0).count();

  std::cout << "n = " << count << std::endl;
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
