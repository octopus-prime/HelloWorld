#include "position.hpp"
#include "square.hpp"
#include "test.hpp"
#include <chrono>
#include <iostream>

#include "bitboard.hpp"

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
  auto count = todo1();
  auto time1 = std::chrono::high_resolution_clock::now();
  auto time = duration_cast<as_floating_point>(time1 - time0).count();

  std::cout << "n = " << count << std::endl;
  std::cout << "t = " << time << std::endl;
  std::cout << "p = " << size_t(count / time) << std::endl;
  std::cout << std::endl;
}

int main() {
  constexpr auto e4_s = "e4"_s2;
  constexpr auto e4_b = "e4"_b2;
  constexpr auto x = bitboard::expand_king(e4_s);
  constexpr auto y = bitboard::expand_kings(e4_b);
  try {
    std::cout << square{"b3"} << std::endl;
    std::cout << bitboard{"f6"_s2} << std::endl;
    std::cout << bitboard{"f6"} << std::endl;
    std::cout << x << std::endl;
    std::cout << y << std::endl;
    //    run();
  } catch (const std::exception &exception) {
    std::cerr << exception.what() << std::endl;
  }
}
